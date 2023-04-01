from __future__ import annotations

from heapq import heapify, heappush, heappop

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

# Find the maximum possible sum of the array after possibly replacing any number with
# a number from a (with running changes) with the number from b for that timestep
#
# Do it like this: keep the current sum and put the numbers in a min-heap. Every time we
# increment time, take the number from b at that timestep iff it is bigger than the minimum
# number in the min-heap, and pop the min-heap's minimum. Make sure to update the sum.
# There are n elements in a and m elements in b, so we'll take O(n(log(n) + log(m))) or at worst
# to construct the min-heap then run through the b's.
def max_sum(a: list[int], b: list[int]) -> int:
    s = sum(a) # Current sum
    # Python's heap is a min-heap by default
    heapify(a)
    for bi in b:
        # Apparently it is forced to make this move
        # if bi > a[0]:
        s -= a[0]
        s += bi
        heappop(a)
        heappush(a, bi)
    return s

# There are t test-cases in total so we'll take O(tn(log(n) + log(m))) in total.
# That will be less than O(100,000*log(100)) iterations, which for log-base 2 is
# at worst O(700,000) iterations which should fall within bounds (hopefully).
T = int(input())
for _ in range(T):
    # For each test case get the first line as n and m if necessary (we don't have to
    # because our language gets the lengths fortunately)
    # and then get the lines for a and b
    #
    _, _ = tuple(map(int, filter(lambda x: len(x) > 0, input().split())))
    a = list(map(int, filter(lambda x: len(x) > 0, input().split())))
    b = list(map(int, filter(lambda x: len(x) > 0, input().split())))
    # print(a)
    # print(b)
    assert len(a) > 0 and len(b) > 0
    print(max_sum(a, b))