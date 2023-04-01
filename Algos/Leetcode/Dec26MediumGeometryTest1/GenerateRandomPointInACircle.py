import random
import math
class Solution:
    # From this: https://leetcode.com/problems/generate-random-point-in-a-circle/
    def __init__(self, radius: float, x_center: float, y_center: float):
        eps = 0.0000001
        assert math.cos(0) == 1 and math.sin(0) == 0
        assert math.fabs(math.cos(math.pi / 2) - 0) < eps and math.fabs(math.sin(math.pi / 2) - 1) < eps

        self.r = radius
        self.x = x_center
        self.y = y_center


    def randPoint(self) -> List[float]:
        angle = random.random() * 2 * math.pi
        rad = random.random() * self.r
        dx = rad * math.cos(angle)
        dy = rad * math.sin(angle)
        x, y = self.x + dx, self.y + dy
        assert (dx**2 + dy**2) <= self.r**2
        return [x, y]


# Your Solution object will be instantiated and called as such:
# obj = Solution(radius, x_center, y_center)
# param_1 = obj.randPoint()