# run with cat C.txt | python3 C.py

import os
import sys
from atexit import register
from io import BytesIO
from heapq import heappush, heappop, heappushpop

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
n, k = list(map(lambda x: int(x.strip()), input().split()))
assert 0 <= k and k <= n
cin = list(map(lambda x: int(x.strip()), input().split()))
assert len(cin) == n

# Setup output DS
cost = 0
cidxout = [None for _ in range(n)]
costOf = lambda idx: cin[idx]

# Load into heap
heap = []
combinedMarginalCost = 0
for i in range(k):
    # NOTE this dumb motherfucker is a minqueue, also I think it doesn't let you use comparator, but I think the tuple => 1st elem wins
    heappush(heap, (-costOf(i), i))
    combinedMarginalCost += costOf(i)
    cost += combinedMarginalCost

# Run greedy algo
for i in range(k, n + k):
    # oprint(f"At idx {i} we have heap is: {' '.join(sorted(map(str, heap)))}") # XXX
    if i >= n:
        # Place most expensive to pay less
        negMostExpensiveCost, mostExpensiveIdx = heappop(heap)
        cidxout[mostExpensiveIdx] = i
        combinedMarginalCost += negMostExpensiveCost # neg!
    else:
        # Replace if necessary, to carry cheapest
        # Index 0 seems to be our starboy
        # https://docs.python.org/3/library/heapq.html
        negMostExpensiveCost, mostExpensiveIdx = heap[0]
        # assert negMostExpensiveCost == min(map(lambda t: t[0], heap)), f"negMostExpensive: {negMostExpensiveCost} was not min at idx {i}"

        mostExpensiveCost = costOf(mostExpensiveIdx)
        # assert mostExpensiveCost == -negMostExpensiveCost, f"mostExpensieCost: {mostExpensiveCost}, negMostExpensiveCost: {negMostExpensiveCost}"

        alternativeIdx = i
        alternativeCost = costOf(alternativeIdx)
        if alternativeCost < mostExpensiveCost:
            heappushpop(heap, (-alternativeCost, i))
            cidxout[mostExpensiveIdx] = i
            combinedMarginalCost += alternativeCost - mostExpensiveCost
            
        else:
            cidxout[i] = i
    
    # Charge for the transport always
    # assert len(heap) == k if i < n else len(heap) == k - (i - (n - 1)), f"i: {i}, n: {n}, k: {k}, len(heap): {len(heap)}"
    assert combinedMarginalCost >= 0
    cost += combinedMarginalCost

# Set up our output
# assert set(cidxout) == set(list(range(k, n+k))), f"cidxout: {cidxout}, list(range(n)): {list(range(k, n+k))}"
# assert all(map(lambda x: x is not None, cidxout)), f"cidxout: {cidxout}"
# assert len(cidxout) == n, f"len(cidxout): {len(cidxout)}, n: {n}"
print(f"{cost}\n".encode('utf-8'))
print((" ".join(map(lambda x: str(x+1), cidxout)) + "\n").encode('utf-8'))

# ...