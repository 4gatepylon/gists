# MATSCodesignalGame

A small Python environment for writing and watching your own policy. The game
is two files: `engine.py` (physics, map loading/generation) and `game.py` (manual
controls, live policy playback, headless evaluation). `policy.py` is your editable
entry point. The engine uses only the standard library; graphics use Pygame.

The implementation plan is deliberately small: define one deterministic engine
API, connect keyboard input and the policy function to that API, generate maps
offline, then verify physics and runner behavior. There is no supplied strategy.

## Run

From this directory, using Python 3.9 or newer:

```sh
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements.txt

python game.py                          # Play manually
python game.py --mode policy            # Watch your policy live
python game.py --mode policy --speed 0.5 # Watch at half speed
python game.py --mode headless          # Evaluate quickly; print a JSON summary
```

Manual controls: **Space** jumps, **P** pauses, **R** restarts, **Esc** quits.
Hold-and-repeat is disabled: release Space before the next press. The screen
shows your path, remaining pickups, fuel, time, and velocity. A completed run stays
visible until restart or quit. Restart also clears policy memory. After editing
`policy.py`, relaunch the process to load the changes.

Headless mode works with plain `python3`, without installing Pygame. Real-time
playback is the MVP's way to watch a run; video export is not implemented.

## Your policy

Edit just this function in `policy.py`:

```python
def policy(observation, memory):
    # Your decision logic goes here.
    return False, memory
```

Return `(jump_this_frame, next_memory)`. The first item must be a Python `bool`.
`True` requests one jump; repeated `True` values request a separate jump each
frame. The engine rejects requests without fuel. `memory` starts as `None`; you
can return any Python object, such as a dictionary, to receive it on the next
frame. You can initialize your state and perform one-time setup on the first call.

The starter deliberately never jumps and will normally crash. It contains no
policy solution or hints.

| Observation field | Meaning |
| --- | --- |
| `x`, `y` | Sprite center in pixels; right/down are positive |
| `vx`, `vy` | Velocity in pixels per second |
| `fuel` | Current integer fuel balance |
| `fuels` | Tuple of remaining pickups, each with `.x` and `.y` |
| `frame`, `time` | Completed simulation steps and elapsed simulation seconds |
| `status`, `done` | `running`, `won`, or `crashed`; whether the run has ended |
| `jumped` | Whether the previous step actually applied a jump |
| `config` | Immutable map, including physics parameters and original pickup list |

Observations and map data are immutable snapshots. The runner calls the policy
once before each simulation step and stops calling it when the run ends. All
pickup locations are visible to your policy from the start.

You can also invoke the engine directly from Python:

```python
from engine import Engine, Map

engine = Engine(Map.load("maps/demo.json"))
observation = engine.observe()
observation = engine.step(jump=False)  # Advance one simulation step
observation = engine.reset()          # Restore the original map and fuel
```

For a complete headless episode with your own callable, use
`game.run_headless(engine, your_policy)`. For live playback, use
`game.run_visual(engine, your_policy)`. Both start a fresh episode.

## Rules and physics

- Horizontal speed is constant. Gravity accelerates downward.
- A successful jump costs exactly one fuel and **sets** vertical velocity to
  `-jump_speed`. It can happen in midair. It does not add an impulse to the
  existing velocity.
- Each pickup adds one fuel and disappears permanently for that run. Fuel has
  no upper limit. Zero fuel alone does not end the run.
- Touching the top or bottom boundary ends the run. Reaching the right boundary
  wins. Contact uses the sprite's radius. The earliest contact wins; a simultaneous
  finish/wall contact is a crash.
- Each step applies the requested jump, moves the sprite, and collects touched
  pickups. Fuel acquired during that movement is available on the next step.
- Simulation uses `dt = 1 / fps`. Within a step, motion follows
  `x += speed * dt`, `y += vy * dt + 0.5 * gravity * dt**2`, then
  `vy += gravity * dt`. A terminal contact shortens the last step.
- Pickups use swept circle collisions, preventing fast movement from skipping
  them. The curved path is approximated with short segments whose deviation is
  at most 1% of the combined collision radius. Boundary contacts are calculated
  from the ballistic trajectory.
- Identical maps and action sequences produce identical engine results.
  Playback speed only changes the wall-clock delay between simulation steps.
  A slow display slows playback instead of skipping decisions.

## Maps

Generate a map **before** starting a game:

```sh
python engine.py maps/practice.json --seed 42 --fuel-count 4
python game.py --map maps/practice.json
python game.py --map maps/practice.json --mode policy
```

The generator writes a JSON file; the game only loads that file. The same seed
and pickup count produce the same map. The generator distributes fuel randomly
inside the canvas; it does not guarantee solvability for arbitrary parameter edits.
Generation replaces the named output file if it already exists.

The default demo and generator use four pickups, three starting fuel, gravity
of 400 pixels/second², and an upward jump speed of 600 pixels/second.

Edit the generated JSON to adjust any parameter:

```json
{
  "canvas_x": 1000,
  "canvas_y": 600,
  "start_x": 40,
  "start_y": 300,
  "start_vy": 0,
  "initial_fuel": 3,
  "speed": 100,
  "gravity": 400,
  "jump_speed": 600,
  "fps": 60,
  "sprite_radius": 12,
  "fuel_radius": 10,
  "fuels": [{"x": 240, "y": 280}, {"x": 420, "y": 320}]
}
```

Distances are pixels, speed is pixels/second, and gravity is pixels/second².
`jump_speed` is the positive upward speed magnitude. `canvas_x` and `canvas_y`
are also the window dimensions. Omitted fields use the defaults above, except
`fuels`, which defaults to an empty tuple. The loader validates parameter types,
dimensions, starting position, and pickup locations.

## Checks

```sh
python3 -m unittest -v
```

Tests cover physics, jump cost and fuel gating, single-use pickups, fast and
curved pickup sweeps, boundary ordering, deterministic runs, map validation and
round trips, immutable observations, and policy memory/reset behavior.
