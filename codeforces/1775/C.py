from __future__ import annotations
from typing import Optional

from math import log2

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

# if we want x to be zero, then we need to add
# - 1 bit at 2^k: 2^k times
# - 2 bits, 2^k_1 + 2^k_2 (k_1 > k_2): 2^k_2 + 2^(k_2+1) + ... + (2^k_1-1)
# - n bits: 1 + sum 2^k_i where k_i is a zero and there is a 1 somewhere to the left of k_i
#   (you can really easily find this just by taking the bitwise complement and then adding 1...
#    aka just taking the negative of the number and treating it as unsigned in 2's complement)
# 
# if you make m bigger then x gets smaller every time you add 2^k_l bits for the lowest bit at k_l
# so if you want to find the m you can just shift x and n right and shift m left (with a 1) until
# we take off a bit and then just continue until you have exhausted the bits
#
# O(1)
def find_adds_zero(n: int) -> int:
    # otherwise we need to add
    # on bit 63 then will move down bit suffix_len + 1 to be 2^suffix_len, then subtract to get all 1's
    assert n > 0
    l = int(log2(n)) + 1
    largest_possible = (2**63 >> (63 - l)) - 1
    assert largest_possible >> (l - 1) == 1 and n >> (l - 1) == 1
    complement = largest_possible - n
    adds = complement + 1
    return adds

def find_m(n: int, x: int) -> int:
    assert n >= 0 and x >= 0 and n < 2**63 and x < 2**63
    # x has to be less than n because it cannot contain more bits (because of ands)
    # x cannot have bits that n does not have (because of the ands)
    # x cannot have a one to the right of zeroes where n has 1's in those zeros (because we can only add)
    #   (i.e. x has to prefix n)
    if x == 0:
        if n == 0:
            return 0
        else:
            return n + find_adds_zero(n)
    if x == n:
        return n
    if n == 0:
        if x == 0:
            return 0
        else:
            return -1
    if x > n or (x | n) > n or (n - x) > x:
        return -1
    suffix = n - x
    prefix = x
    # print("suffix is " + str(suffix) + " and prefix is " + str(prefix))
    if suffix == 0:
        return n
    assert suffix > 0
    # If there is not an empty space for the overflow it won't work
    suffix_len = int(log2(suffix)) + 1
    if (prefix >> suffix_len) & 1 == 1:
        return -1
    return n + find_adds_zero(suffix)

# Shit
T = int(input())
for _ in range(T):
    n, x = tuple(map(int, filter(lambda x: len(x) > 0, input().split())))
    print(find_m(n ,x))