import os
import sys
from atexit import register
from io import BytesIO

# Change sys.stdout to a buffer
sys.stdout = BytesIO()
# `atexit` I'm guessing will make it such that when the program exits (like a trap call on EXIT signal)
# will write to file desriptor 1 (stdout) the value written to sys.stdout, which, remember, we CHANGED
# to be a BytesIO object. This makes it thus that we will write everything at once.
register(lambda: os.write(1, sys.stdout.getvalue()))

# input renamed to be the function that reads one line from the standard input
# use by calling myvar = input() and so forth. NOTE that this returns bytes, not strings
input = BytesIO(os.read(0, os.fstat(0).st_size)).readline

# same trick to rename writing to the stdout buffer
print = sys.stdout.write

# Problem of partial sums is more interesting (using DP)
T = int(input())
for _ in range(T):
    N = int(input())
    ns = list(map(int, input().split(b" ")))

    num_odds = 0
    for i in range(0, len(ns)):
        num_odds += ns[i] % 2
    ans = min(num_odds, N - num_odds)

    print(bytes(f"{ans}\n", "utf-8"))

# ...