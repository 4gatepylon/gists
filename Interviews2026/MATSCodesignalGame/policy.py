"""Write your policy here. This starter deliberately takes no action."""

from typing import Any, Tuple

from engine import Observation


def policy(observation: Observation, memory: Any) -> Tuple[bool, Any]:
    """Return (jump_this_frame, next_memory).

    observation: x, y, vx, vy, fuel, fuels (remaining pickups), frame, time,
                 status, done, jumped (previous step), config (map parameters).
    Coordinates: pixels, positive x right, positive y down; velocities: px/s.
    memory: None on a new run; otherwise exactly what you returned last frame.
    True requests one jump THIS frame, costing one fuel if available.
    """
    return False, memory
