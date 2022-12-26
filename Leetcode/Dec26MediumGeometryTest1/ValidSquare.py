from typing import List
# From: https://leetcode.com/problems/valid-square/
# A lot of iteration on this one, should have spent more time to plan it out beforehand
def d2(p1: List[int], p2: List[int]):
    return (p1[0] - p2[0])**2 + (p1[1] - p2[1])**2
class Solution:
    def validSquare(self, p1: List[int], p2: List[int], p3: List[int], p4: List[int]) -> bool:
        d12_2 = d2(p1, p2)
        d13_2 = d2(p1, p3)
        d23_2 = d2(p2, p3)
        r1, r2 = None, None
        side1_2, side2_2, side3_2, side4_2 = None, None, None, None
        # If diagonal is less tor equal than/to side, definately not square (and will fail because
        # it will think the diagonal is a side and then it won't be equal to the other side)
        if d12_2 > d13_2 and d13_2 == d23_2:
            # diagonal is 12
            # the other diagonal is 34
            # flow 1 -> 4 -> 2 -> 3
            r1, r2 = p1, p2
            side1_2, side2_2, side3_2, side4_2 = d2(p1, p4), d2(p4, p2), d23_2, d13_2,
        elif d13_2 > d12_2 and d12_2 == d23_2:
            # diagonal is 13
            # the other diagonal is 24
            # flow 1 -> 2 -> 3 ->  4
            r1, r2 = p1, p3
            side1_2, side2_2, side3_2, side4_2 = d12_2, d23_2, d2(p4, p3), d2(p1, p4)
        elif d23_2 > d12_2 and d12_2 == d13_2:
            # diagonal is 23
            # the other diagonal is 14
            # flow 2 -> 1 -> 3 -> 4
            r1, r2 = p2, p3
            side1_2, side2_2, side3_2, side4_2 = d12_2, d13_2, d2(p4, p3), d2(p2, p4)
        
        # Make sure that it at least had a triangle half of a square-like parallelogram
        if side1_2 is None:
            assert r1 is None and r2 is None
            assert side2_2 is None and side3_2 is None and side4_2 is None
            return False
        # Make sure it's at least a parallelogram that us square-like
        if side1_2 != side2_2 or side3_2 != side2_2 or side4_2 != side3_2:
            return False
        # Make sure it's a rectangle using the pythagorean theorem
        # (this works because we did a cyclical side-trajectory)
        # (a^2 + a^2 = 2a^2)
        if 2 * side1_2 != d2(r1, r2):
            return False
        return True