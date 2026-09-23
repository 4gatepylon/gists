"""Engine contract tests; no policy strategy is implemented here."""

import json
import tempfile
import unittest
from dataclasses import FrozenInstanceError, replace
from pathlib import Path

from engine import Engine, Map, Pickup, generate_map
from game import run_headless


# Explicit test inputs, independent of gameplay defaults and the demo map.
TEST_MAP = Map(
    canvas_x=1000,
    canvas_y=600,
    start_x=40.0,
    start_y=300.0,
    start_vy=0.0,
    initial_fuel=2,
    speed=100.0,
    gravity=200.0,
    jump_speed=150.0,
    fps=60,
    sprite_radius=12.0,
    fuel_radius=10.0,
    fuels=(),
)


class EngineTests(unittest.TestCase):
    def test_constant_speed_and_gravity(self):
        engine = Engine(TEST_MAP)
        for _ in range(60):
            state = engine.step(False)
        self.assertAlmostEqual(state.time, 1.0)
        self.assertAlmostEqual(state.x, 140.0)
        self.assertAlmostEqual(state.y, 400.0)
        self.assertAlmostEqual(state.vy, 200.0)
        self.assertEqual(state.vx, 100.0)

    def test_jump_sets_velocity_and_costs_exactly_one(self):
        engine = Engine(replace(TEST_MAP, start_vy=70))
        state = engine.step(True)
        self.assertTrue(state.jumped)
        self.assertEqual(state.fuel, 1)
        self.assertAlmostEqual(state.vy, -150 + 200 / 60)
        self.assertAlmostEqual(state.y, 300 - 150 / 60 + 100 / 60 ** 2)
        state = engine.step(True)
        self.assertEqual(state.fuel, 0)
        self.assertAlmostEqual(state.vy, -150 + 200 / 60)

    def test_no_fuel_means_no_jump(self):
        c = replace(TEST_MAP, initial_fuel=0)
        self.assertEqual(Engine(c).step(True), Engine(c).step(False))

    def test_pickup_once_and_only_available_after_movement(self):
        engine = Engine(replace(TEST_MAP, initial_fuel=0, gravity=0,
                                fuels=(Pickup(45, 300),)))
        state = engine.step(True)
        self.assertFalse(state.jumped)
        self.assertEqual(state.fuel, 1)
        self.assertEqual(state.fuels, ())
        self.assertEqual(engine.step(False).fuel, 1)
        self.assertEqual(engine.step(True).fuel, 0)

    def test_fast_motion_cannot_skip_a_pickup(self):
        engine = Engine(replace(TEST_MAP, speed=30000, gravity=0, initial_fuel=0,
                                fuels=(Pickup(200, 300), Pickup(400, 300))))
        state = engine.step(False)
        self.assertEqual(state.fuel, 2)
        self.assertEqual(state.fuels, ())

    def test_curved_sweep_at_low_frame_rate(self):
        c = replace(TEST_MAP, fps=1, gravity=800, start_vy=-400,
                    initial_fuel=0, fuels=(Pickup(90, 200),))
        self.assertEqual(Engine(c).step(False).fuel, 1)

    def test_floor_collision_stops_before_later_pickups(self):
        c = replace(TEST_MAP, start_y=580, start_vy=1000, speed=60000, gravity=0,
                    initial_fuel=0, fuels=(Pickup(800, 580),))
        state = Engine(c).step(False)
        self.assertEqual(state.status, "crashed")
        self.assertAlmostEqual(state.y, 588)
        self.assertAlmostEqual(state.time, 0.008)
        self.assertAlmostEqual(state.x, 520)
        self.assertEqual(state.fuel, 0)

    def test_ceiling_contact_even_if_frame_endpoint_would_be_inside(self):
        state = Engine(replace(TEST_MAP, start_y=13, start_vy=-200, gravity=20000)).step(False)
        self.assertEqual(state.status, "crashed")
        self.assertAlmostEqual(state.y, 12)
        self.assertAlmostEqual(state.time, 0.01)

    def test_earliest_finish_beats_later_wall(self):
        state = Engine(replace(TEST_MAP, start_x=980, start_y=570, start_vy=1000,
                               speed=1000, gravity=0)).step(False)
        self.assertEqual(state.status, "won")
        self.assertAlmostEqual(state.x, 988)
        self.assertAlmostEqual(state.y, 578)

    def test_simultaneous_finish_and_wall_is_a_crash(self):
        state = Engine(replace(TEST_MAP, start_x=980, start_y=580, start_vy=1000,
                               speed=1000, gravity=0)).step(False)
        self.assertEqual(state.status, "crashed")

    def test_terminal_state_is_frozen_and_reset_restores_map(self):
        engine = Engine(replace(TEST_MAP, start_x=980, speed=1000, gravity=0))
        initial = engine.observe()
        terminal = engine.step(False)
        self.assertTrue(terminal.done)
        self.assertIs(engine.step(True), terminal)
        self.assertEqual(engine.reset(), initial)

    def test_observation_and_map_are_immutable(self):
        state = Engine(TEST_MAP).observe()
        with self.assertRaises(FrozenInstanceError):
            state.fuel = 100
        with self.assertRaises(FrozenInstanceError):
            state.config.gravity = 0

    def test_identical_action_sequences_are_deterministic(self):
        config = replace(TEST_MAP, fuels=(Pickup(100, 250), Pickup(200, 400)))
        engines = [Engine(config), Engine(config)]
        # A fixed input fixture, independent of observations, tests determinism.
        for jump in [False, True, False, False, True] + [False] * 200:
            self.assertEqual(engines[0].step(jump), engines[1].step(jump))

    def test_bad_action_has_a_clear_error(self):
        with self.assertRaisesRegex(TypeError, "True or False"):
            Engine(TEST_MAP).step("jump")


class MapAndRunnerTests(unittest.TestCase):
    def test_map_round_trip_and_seed(self):
        config = generate_map(7, 10)
        self.assertEqual(config, generate_map(7, 10))
        self.assertNotEqual(config, generate_map(8, 10))
        with tempfile.TemporaryDirectory() as directory:
            path = str(Path(directory) / "nested" / "map.json")
            config.save(path)
            self.assertEqual(Map.load(path), config)
            self.assertEqual(len(json.loads(Path(path).read_text())["fuels"]), 10)

    def test_bad_map_values(self):
        for kwargs in ({"fps": 0}, {"speed": 0}, {"gravity": -1},
                       {"initial_fuel": -1}, {"initial_fuel": 1.5},
                       {"gravity": float("nan")}, {"start_x": 2000},
                       {"start_y": 12}, {"canvas_x": True},
                       {"fuels": (Pickup(0, 300),)}):
            with self.subTest(kwargs=kwargs), self.assertRaises(ValueError):
                replace(TEST_MAP, **kwargs)

    def test_bad_map_json_shapes(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "bad.json"
            for value in ([], {"unknown": 1}, {"fuels": None}, {"fuels": [{}]}):
                path.write_text(json.dumps(value))
                with self.subTest(value=value), self.assertRaises(ValueError):
                    Map.load(str(path))

    def test_memory_is_carried_between_frames_and_reset_between_runs(self):
        calls = []

        def recording_policy(observation, memory):
            calls.append((observation.frame, memory))
            return False, observation.frame + 1

        engine = Engine(replace(TEST_MAP, start_x=980, gravity=0))
        first = run_headless(engine, recording_policy)
        self.assertEqual(first.status, "won")
        self.assertEqual(calls, [(0, None)] + [(i, i) for i in range(1, first.frame)])
        calls.clear()
        self.assertEqual(run_headless(engine, recording_policy), first)
        self.assertEqual(calls[0], (0, None))

    def test_headless_run_stops_at_a_crash(self):
        def no_action(observation, memory):
            return False, memory

        state = run_headless(Engine(TEST_MAP), no_action)
        self.assertEqual(state.status, "crashed")


if __name__ == "__main__":
    unittest.main()
