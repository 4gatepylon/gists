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

# Wrote this before, it's logarithmic
def gcd(a: int, b: int) -> int:
    if a == b:
        return a
    if a == 1 or b == 1:
        return 1
    assert a != 0 and b != 0
    a, b = max(a, b), min(a, b)
    if a % b == 0:
        return b
    # think about it
    return gcd(b, a % b)

# 1. Create a graph by calculating GCD from each spider to each other spider
#    O(n^2log(s)) where s is the value of the number
# 2. Djikstra's: O(nlogn), but we can brute force because asymptotics come from the operation before
def get_shortest_path(spiders: list[int], s: int, t: int) -> list[int]:
    gcds = [[gcd(spiders[i], spiders[j]) for j in range(i+1, len(spiders))] for i in range(len(spiders))]
    graph = {spider : [] for spider in spiders}
    for i, spider_friends in enumerate(gcds):
        for j, friend_gcd in enumerate(spider_friends):
            if friend_gcd > 1:
                graph[spiders[i]].append(spiders[j])
                graph[spiders[j]].append(spiders[i])
    return [-1] # TODO

# Didn't have time but this is all from https://codeforces.com/contest/1775

# One test case
input()
# pass through a set since you can never get closer by going to someone with the same value
spiders = list(set(map(int, filter(lambda x: len(x) > 0, input().split()))))
s, t = tuple(map(int, filter(lambda x: len(x) > 0, input().split())))
print(" ".join(map(str, get_shortest_path(spiders, s, t))))