from __future__ import annotations

import os
import sys

from atexit import register
from io import BytesIO
from typing import List


# Change sys.stdout to a buffer
old_stdout = sys.stdout
sys.stdout = BytesIO()
# `atexit` I'm guessing will make it such that when the program exits (like a trap call on EXIT signal)
# will write to file desriptor 1 (stdout) the value written to sys.stdout, which, remember, we CHANGED
# to be a BytesIO object. This makes it thus that we will write everything at once.
register(lambda: os.write(1, sys.stdout.getvalue()))

# input renamed to be the function that reads one line from the standard input
# use by calling myvar = input() and so forth. NOTE that this returns bytes, not strings
input = BytesIO(os.read(0, os.fstat(0).st_size)).readline

# same trick to rename writing to the stdout buffer
oprint = lambda x: old_stdout.write(x + "\n")
print = sys.stdout.write

# Get input
n, t = list(map(lambda x: int(x.strip()), input().split()))

MIN_POINT = 1
MAX_POINT = 4
MIN_PATH_LEN = 3
MAX_PATH_LEN = 6

DeltaPath = List[int]
Path = List[int]

# NOTE pure, preprocessing class
class HumpPaths:
    # NOTE this will include the first and last points
    @staticmethod
    def generatePoints(h0: int, p: DeltaPath) -> Path:
        points = [h0]
        for delta in p:
            points.append(points[-1] + delta)
        return points

    # Generate all 6^length possibilities
    # TODO this can be optimized
    @staticmethod
    def generateAllDeltaPathPermutations(length: int) -> list[DeltaPath]:
        if length == 0:
            return []
        allSmallerCombos = HumpPaths.generateAllDeltaPathPermutations(length - 1)
        allCombos = []
        for combo in allSmallerCombos:
            # NOTE this will copy
            allCombos.append(combo + [1])
            allCombos.append(combo + [-1])
            allCombos.append(combo + [2])
            allCombos.append(combo + [-2])
            allCombos.append(combo + [3])
            allCombos.append(combo + [-3])
        return allCombos

    @staticmethod
    def generateAllHumpPathsForHeightLength(h0: int, length: int) -> list[Path]:
        # Detect valid Paths
        def validPath(points: Path) -> bool:
            if not all([MIN_POINT <= point and point <= MAX_POINT for point in points]):
                return False
            if len(points) < MIN_PATH_LEN or len(points) > MAX_PATH_LEN:
                return False
            if points[1] <= points[0]:
                return False
            
            # Must terminate not going up (i.e. going down) and must go up once and then come down once, never parallel to x axis
            going_up = True
            for i in range(1, len(points)):
                if points[i-1] == points[i]:
                    return False
                elif points[i-1] > points[i] and not going_up:
                    return False
                elif points[i-1] < points[i] and going_up:
                    going_up = False
            return not going_up
        
        # Look at all paths and filter for validity
        allPaths = []
        for deltaPath in HumpPaths.generateAllDeltaPathPermutations(length):
            points = HumpPaths.generatePoints(h0, deltaPath)
            assert len(points) == length + 1 # NOTE this is because we assume one point is already placed, so we are eating `length` more points
            if validPath(points):
                allPaths.append(points)
        # NOTE for each path the start point should already be in the total multi-hump path you are creating and the
        # last point will be stored (the last height) in the end of the list
        return allPaths

    @staticmethod
    def generateAllHumpPaths():
        paths = {}
        for height in range(MIN_POINT, MAX_POINT + 1):
            paths[height] = {}
            for length in range(MIN_PATH_LEN, MAX_PATH_LEN + 1):
                paths[height][length] = HumpPaths.generateAllHumpPathsForHeightLength(height, length)
        return paths

# Define our DP algorithm
def countSolutions(n: int, t: int):
    # Sometimes we can't solve the problem at allb (max and min sizes of humps)
    def solvable(n: int, t: int):
        # NOTE just assume that a single point or unidirectional line doesn't count
        return 0 <= n and 0 <= t and (t == 0 and n == 0) or (t > 0 and 2 * t + 1 <= n and n <= 6 * t + 1)
    
    # Early stop
    if not solvable(n, t):
        return 0

    # Precompute for a given starting point (i.e. called a `height` above) and a given length of path to consume
    # (that is to say, assuming we will eat `length` more points out of our n total) how many paths could we take that might
    # be valid (for now)
    print(b"Precomputing all possible paths...\n")
    humpPaths = HumpPaths.generateAllHumpPaths()

    # Create memo
    # Index by [how many points left = n][how many humps left = t][what height we start at = h]
    # Don't include t since we'll condition on each starting point
    # Include 0 for easy indexxing, though it won't be used!
    memo = [[[None for _ in range(0, MAX_POINT + 1)] for _ in range(t)] for _ in range(n + 1)]

    # Fill memo
    print(b"Filling memo...\n")
    for ni in range(0, n+1):
        for ti in range(0, t):
            for hi in range(0, MAX_POINT+1):
                if hi < MIN_POINT:
                    memo[ni][ti][hi] = -1
                else:
                    assert 0 <= ni and ni <= n
                    assert 0 <= ti and ti <= t
                    assert MIN_POINT <= hi and hi <= MAX_POINT
                    memo[ni][ti][hi] = 0
                    # NOTE solvable is not that rigorous here because it doesn't use height which can matter
                    if solvable(ni, ti):
                        if ni == 0 and ti == 0:
                            memo[ni][ti][hi] = 1
                        else:
                            for hump_len, hump in humpPaths[hi]:
                                hump_height_final = hump[-1]
                                if solvable(ni - hump_len, ti - 1):
                                    assert 0 <= ni - hump_len and ni - hump_len <= n
                                    assert 0 <= ti - 1 and ti - 1 <= t
                                    assert MIN_POINT <= hump_height_final and hump_height_final <= MAX_POINT
                                    memo[ni][ti][hi] += memo[ni - hump_len][ti - 1][hump_height_final]

    # MAke sure we didn't fuck up anywhere
    assert all([all([all([elem is not None or elem == -1 for elem in col]) for col in row]) for row in memo])

    # NOTE OOPS had to lower to t - 1 because in this framework we are fixing the initial height
    return sum([memo[n][t - 1][h] for h in range(MIN_POINT, MAX_POINT + 1)])
    

# Generate our output
print(f'{countSolutions(n, t)}\n'.encode('utf-8'))

# NOTE ran out of time!
# `cat camels.txt | python3 camels.py`

# TODO
# 1. Visualize memo
# 2. Improve precomputing and visualize hump cache
# 3. Optimize solvability checker(s)
# 4. Get it to work...