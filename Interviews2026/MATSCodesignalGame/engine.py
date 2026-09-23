"""Deterministic game physics and offline JSON map generation (standard library only)."""

import argparse
import json
import math
import random
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Any, Dict, Optional, Tuple


@dataclass(frozen=True)
class Pickup:
    x: float
    y: float


@dataclass(frozen=True)
class Map:
    canvas_x: int = 1000
    canvas_y: int = 600
    start_x: float = 40.0
    start_y: float = 300.0
    start_vy: float = 0.0
    initial_fuel: int = 3
    speed: float = 100.0
    gravity: float = 400.0
    jump_speed: float = 600.0
    fps: int = 60
    sprite_radius: float = 12.0
    fuel_radius: float = 10.0
    fuels: Tuple[Pickup, ...] = ()

    def __post_init__(self) -> None:
        for name in ("canvas_x", "canvas_y", "initial_fuel", "fps"):
            value = getattr(self, name)
            if type(value) is not int or value < (0 if name == "initial_fuel" else 1):
                raise ValueError("{} must be an integer >= {}".format(
                    name, 0 if name == "initial_fuel" else 1))
        for name in ("start_x", "start_y", "start_vy", "speed", "gravity",
                     "jump_speed", "sprite_radius", "fuel_radius"):
            value = getattr(self, name)
            if type(value) not in (int, float) or not math.isfinite(value):
                raise ValueError("{} must be a finite number".format(name))
        for name in ("speed", "jump_speed", "sprite_radius", "fuel_radius"):
            if getattr(self, name) <= 0:
                raise ValueError("{} must be positive".format(name))
        if self.gravity < 0:
            raise ValueError("gravity must be nonnegative")
        r = self.sprite_radius
        if not (r <= self.start_x < self.canvas_x - r and
                r < self.start_y < self.canvas_y - r):
            raise ValueError("start position must fit inside the canvas")
        if type(self.fuels) is not tuple:
            raise ValueError("fuels must be a tuple of Pickup objects")
        for fuel in self.fuels:
            if not isinstance(fuel, Pickup):
                raise ValueError("each fuel must be a Pickup")
            if any(type(v) not in (int, float) or not math.isfinite(v)
                   for v in (fuel.x, fuel.y)):
                raise ValueError("fuel coordinates must be finite numbers")
            if not (self.fuel_radius <= fuel.x <= self.canvas_x - self.fuel_radius
                    and self.fuel_radius <= fuel.y <= self.canvas_y - self.fuel_radius):
                raise ValueError("fuel pickups must fit inside the canvas")

    @classmethod
    def load(cls, path: str) -> "Map":
        with open(path) as stream:
            data = json.load(stream)
        if not isinstance(data, dict):
            raise ValueError("map JSON must be an object")
        try:
            data["fuels"] = tuple(Pickup(**p) for p in data.get("fuels", []))
            return cls(**data)
        except TypeError as exc:
            raise ValueError("invalid map fields: {}".format(exc)) from exc

    def save(self, path: str) -> None:
        target = Path(path)
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(json.dumps(asdict(self), indent=2) + "\n")


@dataclass(frozen=True)
class Observation:
    x: float
    y: float
    vx: float
    vy: float
    fuel: int
    fuels: Tuple[Pickup, ...]
    frame: int
    time: float
    status: str
    jumped: bool
    config: Map

    @property
    def done(self) -> bool:
        return self.status != "running"


def _boundary_time(y: float, vy: float, gravity: float,
                   boundary: float, dt: float) -> Optional[float]:
    """Earliest contact with a horizontal boundary during this tick."""
    if gravity == 0:
        roots = ((boundary - y) / vy,) if vy else ()
    else:
        discriminant = vy * vy - 2 * gravity * (y - boundary)
        if discriminant < 0:
            return None
        root = math.sqrt(discriminant)
        # Stable quadratic roots, including tiny distances from a boundary.
        q = -0.5 * (vy + math.copysign(root, vy))
        roots = (q / (0.5 * gravity), (y - boundary) / q) if q else (0.0,)
    candidates = [t for t in roots if 0 <= t <= dt]
    return min(candidates) if candidates else None


def _touches(x0: float, y0: float, x1: float, y1: float,
             pickup: Pickup, radius: float) -> bool:
    """Swept circle pickup: test the whole movement segment, not just its end."""
    dx, dy = x1 - x0, y1 - y0
    length_squared = dx * dx + dy * dy
    t = (((pickup.x - x0) * dx + (pickup.y - y0) * dy) / length_squared
         if length_squared else 0.0)
    t = max(0.0, min(1.0, t))
    return (pickup.x - x0 - t * dx) ** 2 + (pickup.y - y0 - t * dy) ** 2 <= radius ** 2


class Engine:
    """Call observe(), then step(jump: bool) once per simulation tick."""

    def __init__(self, config: Map):
        self.config = config
        self.reset()

    def reset(self) -> Observation:
        c = self.config
        self._state = Observation(c.start_x, c.start_y, c.speed, c.start_vy,
                                  c.initial_fuel, c.fuels, 0, 0.0, "running", False, c)
        return self._state

    def observe(self) -> Observation:
        return self._state

    def step(self, jump: bool = False) -> Observation:
        if type(jump) is not bool:
            raise TypeError("jump must be True or False")
        s, c = self._state, self.config
        if s.done:
            return s
        jumped = jump and s.fuel >= 1
        fuel = s.fuel - int(jumped)
        vy = -c.jump_speed if jumped else s.vy
        dt = 1.0 / c.fps

        # Resolve the earliest terminal contact, so pickups beyond it do not count.
        # A simultaneous wall/finish contact is a crash.
        events = []
        finish_time = (c.canvas_x - c.sprite_radius - s.x) / c.speed
        if 0 <= finish_time <= dt:
            events.append((finish_time, 1, "won"))
        for boundary in (c.sprite_radius, c.canvas_y - c.sprite_radius):
            contact = _boundary_time(s.y, vy, c.gravity, boundary, dt)
            if contact is not None:
                events.append((contact, 0, "crashed"))
        duration, _, status = min(events) if events else (dt, 2, "running")
        x = s.x + c.speed * duration
        y = s.y + vy * duration + 0.5 * c.gravity * duration ** 2

        # Short chord segments approximate the curved pickup sweep. Their maximum
        # deviation from the ballistic path is <= 1% of the collision radius.
        radius = c.sprite_radius + c.fuel_radius
        segments = max(1, math.ceil(duration * math.sqrt(c.gravity / (0.08 * radius))))
        remaining = list(s.fuels)
        old_x, old_y = s.x, s.y
        for i in range(1, segments + 1):
            t = duration * i / segments
            new_x = s.x + c.speed * t
            new_y = s.y + vy * t + 0.5 * c.gravity * t * t
            remaining = [p for p in remaining
                         if not _touches(old_x, old_y, new_x, new_y, p, radius)]
            old_x, old_y = new_x, new_y
        fuel += len(s.fuels) - len(remaining)
        self._state = Observation(x, y, c.speed, vy + c.gravity * duration,
                                  fuel, tuple(remaining), s.frame + 1,
                                  s.time + duration, status, jumped, c)
        return self._state


def generate_map(seed: int = 0, fuel_count: int = 4) -> Map:
    """Create repeatable pickup positions offline; does not solve the map."""
    if fuel_count < 0:
        raise ValueError("fuel_count must be nonnegative")
    rng = random.Random(seed)
    c = Map()
    fuels = tuple(Pickup(round(rng.uniform(100, c.canvas_x - 50), 2),
                         round(rng.uniform(100, c.canvas_y - 100), 2))
                  for _ in range(fuel_count))
    return Map(fuels=tuple(sorted(fuels, key=lambda p: p.x)))


def summarize(observation: Observation) -> Dict[str, Any]:
    return {"status": observation.status, "time": round(observation.time, 3),
            "frames": observation.frame, "x": round(observation.x, 3),
            "y": round(observation.y, 3), "fuel": observation.fuel,
            "collected": len(observation.config.fuels) - len(observation.fuels)}


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate an offline JSON map.")
    parser.add_argument("output", help="Output map filename")
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--fuel-count", type=int, default=4)
    args = parser.parse_args()
    try:
        generate_map(args.seed, args.fuel_count).save(args.output)
    except (ValueError, OSError) as error:
        parser.error(str(error))
    print("Wrote {}".format(args.output))
