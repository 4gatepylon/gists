# -- TEMPLATE --
import os
import sys
from atexit import register
from io import BytesIO
sys.stdout = BytesIO()
register(lambda: os.write(1, sys.stdout.getvalue()))
input = BytesIO(os.read(0, os.fstat(0).st_size)).readline
print = sys.stdout.write
# -- END TEMPLATE --

# The values are decimals... though it seems like it should be the same for any base (I originally misunderstood
# and did it in base 2).
# Every xy counts for 10*x + y
# Every x THAT IS EQUAL TO 1 counts like this
# If it's on the left endpoint it counts for 10
# If it's on the right endpoint it counts for 1
# If it's in the middle it counts for 11

# Therefore it's optimal to move everything non-zero to the two endpoints
# 1st to the right
# 2nd to the left
# We want zeros except for at the endpoint

# TODO too buggy!
T = int(input())
for _ in range(T):
    N, K = tuple(map(int, (input().split(b" "))))
    # Ascii, 48 = 0, 49 = 1
    bs = input().strip(b"\n")
    Z = 48
    bs = list(map(lambda x: int(x) - Z, bs))
    assert(min(bs) == 0 or min(bs) == 1)
    assert(max(bs) == 0 or max(bs) == 1)
    sum = 0
    if bs[0] == 1:
        sum += 10
    if bs[-1] == 1:
        sum += 1
    for i in range(1, N-1):
        if bs[i] == 1:
            sum += 11
    ans = sum
    # With a good strategy you never need to move more than N
    bank = min(K, N)
    # Prioritize filling the right endpoint first since it loses more value in total
    # NOTE when we do the swap, we only need to swap the 0 and 1 because we pick the
    # rightmost and leftmost 1's, meaning that we don't meaningful modify the array otherwise.
    assert(len(bs) == N)
    assert(len(bs) >= 2)
    assert(N >= bank)
    assert(bs[0] == 0 or bs[0] == 1)
    assert(bs[-1] == 0 or bs[-1] == 1)
    if bank > 0 and bs[-1] == 0:
        rightmost1 = N - 1
        while rightmost1 > 0 and bs[rightmost1] == 0:
            rightmost1 -= 1
        if bs[rightmost1] == 1:
            dist = (N - 1) - rightmost1
            if dist <= bank:
                bank -= dist
                bs[rightmost1] = 0
                bs[-1] = 1
                # Careful if we shift the left endpoint (edge case)
                ans -= (10 if rightmost1 == 0 else 11)
                ans += 1
                assert(bank >= 0)
    if bank > 0 and bs[0] == 0:
        leftmost1 = 0
        while leftmost1 < N - 1 and bs[leftmost1] == 0:
            leftmost1 += 1
        if bs[leftmost1] == 1:
            dist = leftmost1
            # Do NOT shift back the right endpoint since that's a losing move
            if dist <= bank and leftmost1 < N - 1:
                bank -= dist
                bs[leftmost1] = 0
                bs[0] = 1
                assert(leftmost1 != N - 1)
                ans -= 11
                ans += 10
                assert(bank >= 0)
    print(bytes(f"{ans}\n", "utf-8"))