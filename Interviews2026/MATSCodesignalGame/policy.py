"""Use Q-iteration/DP for this problem.

While the space (x/y) is continuous, it the reachable points are discrete from the starter position since the frames
are discretized. You can therefore keep a dictionary basically mapping from (x, y, T, A) to q-values. The rewards are
+1 if you reach the end of the map and 0 otherwise. If the game ends at the top or bottom, your Actions all yield 0 reward
and are modeled as sending you to the same place (i.e. they are absorbing states).

x_new = x + vx * dt
y_new = y + vy * dt + 0.5 * gravity * dt**2
vy_new = vy + gravity * dt
"""

from typing import Any, Tuple, Dict

from engine import Observation, Map

from dataclasses import dataclass


@dataclass(frozen=True)
class State:
    """
    State space size should be roughly aroound (approx):
        (canvas_x) *
        (canvas_y) *
        (max_vx - min_vx) *
        (max_vy - min_vy) *
        (max_T - min_T)

    TODO(hadriano) I think it's more like 2**T_lookahead since there's exactly 2 new states reachable at each point
    """

    x: float  # x axis on the map
    y: float  # y axis on the map
    vx: float  # x velocity
    vy: float  # y velocity
    T: int  # Frame number


@dataclass(frozen=True)
class StateActionPair:
    state: State
    A: bool  # Action taken (spacebar or not; True = spacebar, False = no spacebar)

    def __hash__(self) -> int:
        return hash((self.x, self.y, self.T, self.A))


@dataclass(frozen=True)
class Ball:
    x: float
    y: float
    radius: float

    def __hash__(self) -> int:
        return hash((self.x, self.y, self.radius))

    def __eq__(self, other: "Ball") -> bool:
        return self.x == other.x and self.y == other.y and self.radius == other.radius

    def intersects(self, other: "Ball") -> bool:
        return (self.x - other.x) ** 2 + (self.y - other.y) ** 2 <= (
            self.radius + other.radius
        ) ** 2


def _next_state(
    current_state: State, action: bool, dt: float, gravity: float, jump_speed: float
) -> State:
    # Next positions
    next_x_if_no_jump = current_state.x + current_state.vx * dt
    next_x_if_jump = next_x_if_no_jump  # vx is constant
    next_y_if_no_jump = current_state.y + current_state.vy * dt + 0.5 * gravity * dt**2
    next_y_if_jump = next_y_if_no_jump + jump_speed * dt
    # Next velocities
    next_vx_if_no_jump = current_state.vx
    next_vx_if_jump = current_state.vx  # vx is constant
    next_vy_if_jump = jump_speed
    next_vy_if_no_jump = current_state.vy + gravity * dt
    # Hydrate states
    next_state_if_no_jump = State(
        next_x_if_no_jump,
        next_y_if_no_jump,
        next_vx_if_no_jump,
        next_vy_if_no_jump,
        current_state.T + 1,
    )
    next_state_if_jump = State(
        next_x_if_jump,
        next_y_if_jump,
        next_vx_if_jump,
        next_vy_if_jump,
        current_state.T + 1,
    )
    # TODO(hadriano find a good wy to deal with finding next state)
    if action:  # Jump
        next_state = next_state_if_jump
    else:
        next_state = next_state_if_no_jump
    return next_state


def Q(
    map: Map,
    lookahead_T: int,
    current_state: State,
    action: bool,  # True = jump, False = no jump
    y_offset_top: float = 2.0,
    y_offset_bot: float = 2.0,
    x_offset_left: float = 2.0,
    Q_at_T_0: float = 1.0,
    Q_at_intersect_ball: float = 2.0,
    Q_at_win: float = 4.0,
    Q_at_lose: float = 0.0,
    Q_cache: Dict[StateActionPair, float] = {},
) -> float:
    # 1. Calculate trivial Q-values
    # > 1.1: When you die/lose
    max_allowable_y = map.canvas_y - map.sprite_radius - y_offset_bot
    min_allowable_y = map.sprite_radius + y_offset_top
    if current_state.y > max_allowable_y:
        return Q_at_lose
    elif current_state.y < min_allowable_y:
        return Q_at_lose
    # > 1.2: When you win
    max_non_winning_x = map.canvas_x - map.sprite_radius - x_offset_left
    if current_state.x > max_non_winning_x:
        return Q_at_win
    # > 1.3 When we run out of computation budget
    if lookahead_T < 0:
        return Q_at_T_0
    # > 1.4 When you intersect the ball
    ball = Ball(current_state.x, current_state.y, map.sprite_radius)
    if any(
        ball.intersects(Ball(fuel.x, fuel.y, map.fuel_radius)) for fuel in map.fuels
    ):
        return Q_at_intersect_ball
    # 2. Calculate Q-values using DP
    dt = 1.0 / map.fps
    next_state = _next_state(current_state, action, dt, map.gravity, map.jump_speed)
    if next_state in Q_cache:
        return Q_cache[next_state]
    q_next_state_jump = Q(
        map=map,
        lookahead_T=(lookahead_T - 1),
        current_state=next_state,
        action=True,
        y_offset_top=y_offset_top,
        y_offset_bot=y_offset_bot,
        x_offset_left=x_offset_left,
        Q_cache=Q_cache,
    )
    q_next_state_no_jump = Q(
        map=map,
        lookahead_T=(lookahead_T - 1),
        current_state=next_state,
        action=False,
        y_offset_top=y_offset_top,
        y_offset_bot=y_offset_bot,
        x_offset_left=x_offset_left,
        Q_cache=Q_cache,
    )
    Q_cache[next_state] = max(q_next_state_jump, q_next_state_no_jump)
    return Q_cache[next_state]


def policy(observation: Observation, memory: Any) -> Tuple[bool, Any]:
    """Return (jump_this_frame, next_memory).

    observation: x, y, vx, vy, fuel, fuels (remaining pickups), frame, time,
                 status, done, jumped (previous step), config (map parameters).
    Coordinates: pixels, positive x right, positive y down; velocities: px/s.
    memory: None on a new run; otherwise exactly what you returned last frame.
    True requests one jump THIS frame, costing one fuel if available.
    """
    cache = {}
    current_state = State(
        x=observation.x,
        y=observation.y,
        vx=observation.vx,
        vy=observation.vy,
        T=observation.frame,
    )
    q_no_jump = Q(
        map=observation.config,
        lookahead_T=20,
        current_state=current_state,
        action=False,
        Q_cache=cache,
    )
    q_jump = Q(
        map=observation.config,
        lookahead_T=20,
        current_state=current_state,
        action=True,
        Q_cache=cache,
    )
    if q_no_jump > q_jump:
        action = False  # No jump <=> false
    else:
        action = True  # Jump <=> true
    # from pprint import pprint
    # pprint(cache)
    return action, cache
