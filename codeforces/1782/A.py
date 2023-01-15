from __future__ import annotations

# -- TEMPLATE --
import os
import sys
from atexit import register
from io import BytesIO
sys.stdout = BytesIO()
# THIS WILL MAKE IT PRINT EVERYTHING ALL AT ONCE IN THE END
register(lambda: os.write(1, sys.stdout.getvalue()))
input = BytesIO(os.read(0, os.fstat(0).st_size)).readline
print = lambda x: sys.stdout.write(bytes(f"{x}\n", "utf-8"))
# -- END TEMPLATE --

# the distance will always be the manhattan distance plus the height plus twice the minimum deviation to hit a wall once
# of any optimal manhattan path (so that's either the wallc closest to the laptop or the wall closest to the projector)
def shortest_dist(laptop: tuple[int, int], projector: tuple[int, int], h: int, box_boundaries: tuple[int, int]) -> int:
    a, b = laptop
    f, g = projector
    manhattan_dist = abs(a - f) + abs(b - g)

    w, d = box_boundaries
    smallest_deviation = min(a, w - a, f, w - f, b, d - b, g, d - g)

    return manhattan_dist + h + 2 * smallest_deviation

T = int(input())
for _ in range(T):
    # For each test case get the first line as n and m if necessary (we don't have to
    # because our language gets the lengths fortunately)
    # and then get the lines for a and b
    #
    w, d, h = tuple(map(int, filter(lambda x: len(x) > 0, input().split())))
    a, b, f, g = tuple(map(int, filter(lambda x: len(x) > 0, input().split())))
    assert 0 < w and 0 < d and 0 <= h
    assert 0 < f and f < w and 0 < a and a < w
    assert 0 < g and g < d and 0 < b and b < d
    print(shortest_dist((a, b), (f, g), h, (w, d)))