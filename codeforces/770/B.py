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

# Out of all n! permutations of length n, find the one such that the max cost of any window 
# (of size k, i.e. including k elements) is minimized. The cost of a window is the max element
# in that window plus the min element in that window. We'd like to keep the max and mins low.

# Some ideas
# k = 1, n => any permutation works
# k = 2 => seems like you want to interleave maxes and mins
#   123 has cost max(3,5) = 5
#   213 has cost 213 has cost max(3, 4) = 4
#   1234 has cost 7
#   4132 has cost max(5, 4, 5) = 5
#   etc...
#   (i.e. in this case we can guarantee that the cost of any window is at most twice the cost of the max)
# k = 3 => 
#
# k = n - 1 => same cost as k = 2 because we have max and min together (i.e. max and min together on one side
#   and then the other side loses the max by our construction)
# k = n - 2 => 
#
# It seems like a greedy construction is
#  1st window = k - 1 largest elements in [n] follow-ed by the minimum
#  2nd window = 1st window, but we lose the maximum and add the next largest possible element
#  ... repeat until we are about to lose the minimum element
#      add the second least element
# 
# => this algorithm can guarantee us at most max + min of array cost for any window size...
#    is this the best possible? certainly! it is not possible to do better than this for k > 1
#    becaue each window must have two elements, and one window must have the maximum, so the best
#    in that window is exactly the global max + the global min, and the problem wants us to optimize
#    worst case, so if we can get this as our worst case, then we cannot do better because in the
#    worst case because it is not possible to do better at all for the window with teh max which must
#    exist
#  
# So now we just need to construct the permutation, how to do that? two pointers!
# one pointer at max and one pointer at least, consume from right until must consume from left,
# then repeat until done
def min_perm(n: int, k: int) -> list[int]:
    ans = []
    l, r = 1, n
    cnt = 0
    while l <= r:
        if cnt == k - 1:
            ans.append(l)
            l += 1
            cnt = 0
        else :
            ans.append(r)
            r -= 1
            cnt += 1
    return ans

T = int(input())
for _ in range(T):
    n, k = tuple(map(int, filter(lambda x: len(x) > 0, input().split())))
    mp = min_perm(n, k)
    print(" ".join(map(str, mp)))