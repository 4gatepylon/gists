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

# Things noticed:
# 1. Adding a number to two pairs of integers that are already GCD 1 is not necessarily still GCD 1
#    (i.e. adding k to two numbers -k mod p for some p, like 11 and 17 are both -1 mod 3)
# 2. Adding a multiple of the same number multiple times does not necessarily keep the GCD above
#    the 1st GCD found (for example, 11 and 17, add one twice, you get two primes again, so the GCD went up
#    then back down)
# 3. Any time you have a number x such that the GCD of a pair is > 1 then you should never pick that number plus any
#    multiple of the GCD again.
# 4. If there is a number greater than the max of the pair of numbers we could add, then there is a number smaller
#    than the pair of numbers we could add (such that the GCD is == 1) because ... TODO
# NOT TRUE 5. If you have two numbers that have a non-1 GCD, you can add 1 or any relatively-prime number to both of them
#    to yield a GCD of 1, because ... TODO
#
#
#
#
# What would be nice
# 1. A very small set of numbers to test
# 2. For that very small set (logarithmic in the size of the numbers) we can test all nC2 pairs
# 
# If the properties 1 and 2 from things noticed were true maybe we could have achieved something, but here it seems not...
# ...
# Looking at 3 and 4, if 4 were true then we could just iterate from 1 to the max number, skipping
# invalid values and every time we hit an invalid value, we lose 1/(GCD found) < 1/(max number) of the values,
# meaning that if we call that number m, and we have therefore m numbers to start with, we take
# log(max_number)/log(1 - 1/max_number) which is a terrible constant... TODO
#
# Looking at 5, if 5 were true then we would just need a number that for all pairs of numbers with GCD != 1
# was relatively prime to all of them. This could be any prime number, so this cannot be true because otherwise
# this problem would be dumb... unless it' failing only in the cases with two numbers that are the same?
#
def existsx(a: list[int]) -> bool:
    s = set()
    for ai in a:
        if ai in s:
            return False
        s.add(ai)
    return True

T = int(input())
for _ in range(T):
    input()
    a = list(map(int, filter(lambda x: len(x) > 0, input().split())))
    print("YES" if existsx(a) else "NO")