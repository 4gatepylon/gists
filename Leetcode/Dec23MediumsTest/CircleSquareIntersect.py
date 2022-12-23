import itertools
# From problem https://leetcode.com/problems/circle-and-rectangle-overlapping/description/

class Solution:
    def in_rect(corners: tuple[int, int, int, int], point: tuple[int, int]) -> bool:
        x1, y1, x2, y2 = corners
        x, y = point
        # Has to be included in both dimensions
        return x1 <= x and x <= x2 and y1 <= y and y <= y2
    def in_semi(semi: tuple[tuple[int, int], tuple[int, int]], r2: int, point: tuple[int, int]) -> bool:
        (right, cx), (up, cy) = semi
        x, y = point
        if right and x < cx or not right and x > cx:
            # Has to be to the right side of the quarter circle/s quarter-cutting radius vectors
            return False
        if up and y < cy or not up and y > cy:
            # Has to be to the right side of the quarter circle/s quarter-cutting radius vectors
            return False
        # Has to be close enough
        return (cx - x)**2 + (cy - y)**2 <= r2
    def checkOverlap(self, r: int, xCenter: int, yCenter: int, x1: int, y1: int, x2: int, y2: int) -> bool:
        # They overlap if the circle's center is in the rounded rectangle around the rectangle or if the
        # rectangle's center is in some sort of other relation with the circle... let's do the firs one
        horiz_cross = (x1 - r, y1, x2 + r, y2)
        vert_cross = (x1, y1 - r, x2, y2 + r)
        rects = [horiz_cross, vert_cross]
        semis = list(itertools.product(enumerate([x1, x2]), enumerate([y1, y2])))
        center = (xCenter, yCenter)
        r2 = r**2
        return (
            any([Solution.in_rect(rect, center) for rect in rects]) or
            any([Solution.in_semi(semi, r2, center) for semi in semis])
        )