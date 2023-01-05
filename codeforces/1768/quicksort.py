from __future__ import annotations

import math

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

def qs(p: list[int], k: int) -> int:
    n = len(p)
    # Keep track of the largest subsequence in the beginning that is ordered
    ordered = [False for _ in range(len(p) + 1)]
    ordered[0] = True
    mordered = 1
    for elem in p:
        if ordered[elem - 1]:
            ordered[elem] = True
            mordered = max(mordered, elem)
    return math.ceil((n - mordered) / k)

T = int(input())
for _ in range(T):
    # For each test case get the first line as n and m if necessary (we don't have to
    # because our language gets the lengths fortunately)
    # and then get the lines for a and b
    #
    _, k = tuple(map(int, filter(lambda x: len(x) > 0, input().split())))
    perm = list(map(int, filter(lambda x: len(x) > 0, input().split())))
    print(qs(perm, k))