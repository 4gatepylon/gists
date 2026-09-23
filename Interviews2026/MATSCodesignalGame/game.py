"""Manual play, live policy playback, and headless policy evaluation."""

import argparse
import json
from pathlib import Path
from typing import Any, Callable, Optional, Tuple

from engine import Engine, Map, Observation, summarize


Policy = Callable[[Observation, Any], Tuple[bool, Any]]


def run_headless(engine: Engine, policy_fn: Policy) -> Observation:
    """Run a fresh episode as fast as possible, with no graphics dependency."""
    observation = engine.reset()
    memory = None
    while not observation.done:
        jump, memory = policy_fn(observation, memory)
        observation = engine.step(jump)
    return observation


def run_visual(engine: Engine, policy_fn: Optional[Policy] = None,
               playback_speed: float = 1.0) -> Observation:
    """Watch a policy, or play with Space when policy_fn is None."""
    import pygame

    pygame.init()
    try:
        c = engine.config
        screen = pygame.display.set_mode((c.canvas_x, c.canvas_y))
        pygame.display.set_caption("MATSCodesignalGame — {}".format(
            "policy" if policy_fn else "manual"))
        font = pygame.font.Font(None, 25)
        big_font = pygame.font.Font(None, 48)
        clock = pygame.time.Clock()
        pygame.key.set_repeat()  # Holding Space must not spend fuel repeatedly.
        observation = engine.reset()
        memory = None
        paused = False
        trail = [(observation.x, observation.y)]
        reported = False

        while True:
            jump = False
            restarted = False
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    return observation
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        return observation
                    if event.key == pygame.K_r:
                        observation = engine.reset()
                        memory, paused, reported = None, False, False
                        trail = [(observation.x, observation.y)]
                        restarted = True
                    elif event.key == pygame.K_p:
                        paused = not paused
                    elif event.key == pygame.K_SPACE and policy_fn is None and not paused:
                        jump = True

            if not observation.done and not paused and not restarted:
                if policy_fn is not None:
                    jump, memory = policy_fn(observation, memory)
                observation = engine.step(jump)
                trail.append((observation.x, observation.y))

            screen.fill((19, 24, 36))
            for x in range(0, c.canvas_x, 50):
                pygame.draw.line(screen, (31, 38, 52), (x, 0), (x, c.canvas_y))
            for y in range(0, c.canvas_y, 50):
                pygame.draw.line(screen, (31, 38, 52), (0, y), (c.canvas_x, y))
            pygame.draw.line(screen, (245, 112, 112), (0, 0), (c.canvas_x, 0), 3)
            pygame.draw.line(screen, (245, 112, 112), (0, c.canvas_y - 1),
                             (c.canvas_x, c.canvas_y - 1), 3)
            pygame.draw.line(screen, (117, 228, 166), (c.canvas_x - 1, 0),
                             (c.canvas_x - 1, c.canvas_y), 3)
            if len(trail) > 1:
                pygame.draw.lines(screen, (70, 106, 141), False, trail, 2)
            for pickup in observation.fuels:
                center = (round(pickup.x), round(pickup.y))
                pygame.draw.circle(screen, (248, 202, 82), center, round(c.fuel_radius))
                pygame.draw.line(screen, (60, 48, 25), (center[0] - 4, center[1]),
                                 (center[0] + 4, center[1]), 2)
                pygame.draw.line(screen, (60, 48, 25), (center[0], center[1] - 4),
                                 (center[0], center[1] + 4), 2)
            color = (248, 129, 115) if observation.status == "crashed" else (112, 207, 255)
            center = (round(observation.x), round(observation.y))
            pygame.draw.circle(screen, color, center, round(c.sprite_radius))
            pygame.draw.circle(screen, (19, 24, 36), (center[0] + 4, center[1] - 3), 2)

            mode = "POLICY" if policy_fn else "MANUAL"
            hud = "{}   Fuel: {}   Time: {:.2f}s   x: {:.0f}   vy: {:.1f}".format(
                mode, observation.fuel, observation.time, observation.x, observation.vy)
            screen.blit(font.render(hud, True, (233, 239, 248), (19, 24, 36)), (14, 14))
            controls = "Space: jump  |  P: pause  |  R: restart  |  Esc: quit"
            if policy_fn:
                controls = "P: pause  |  R: restart  |  Esc: quit"
            screen.blit(font.render(controls, True, (179, 192, 211), (19, 24, 36)),
                        (14, c.canvas_y - 28))
            if observation.done or paused:
                label = observation.status.upper() if observation.done else "PAUSED"
                surface = big_font.render(label, True, (255, 255, 255), (19, 24, 36))
                screen.blit(surface, surface.get_rect(center=(c.canvas_x // 2, c.canvas_y // 2)))
            pygame.display.flip()

            if observation.done and not reported:
                print(json.dumps(summarize(observation)))
                reported = True
            # Wall-clock pacing never changes the physics timestep. On a slow
            # computer playback slows down instead of skipping policy decisions.
            clock.tick(max(1, round(c.fps * playback_speed)))
    finally:
        pygame.quit()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--map", default=str(Path(__file__).parent / "maps" / "demo.json"))
    parser.add_argument("--mode", choices=("manual", "policy", "headless"), default="manual")
    parser.add_argument("--speed", type=float, default=1.0, help="Playback speed, e.g. 0.5 or 2")
    args = parser.parse_args()
    if not 0.05 <= args.speed <= 10:
        parser.error("--speed must be between 0.05 and 10")
    try:
        engine = Engine(Map.load(args.map))
    except (ValueError, OSError) as error:
        parser.error(str(error))
    policy_fn = None
    if args.mode != "manual":
        from policy import policy
        policy_fn = policy
    if args.mode == "headless":
        print(json.dumps(summarize(run_headless(engine, policy_fn))))
    else:
        try:
            run_visual(engine, policy_fn, args.speed)
        except ModuleNotFoundError as error:
            if error.name != "pygame":
                raise
            parser.exit(1, "Visual mode needs pygame: python -m pip install -r requirements.txt\n")


if __name__ == "__main__":
    main()
