from typing import List
# From https://leetcode.com/problems/minimum-lines-to-represent-a-line-chart/description/

class Solution:
    def minimumLines(self, stockPrices: List[List[int]]) -> int:
        # Sort by x so we can just go through it greedily
        stockPrices = sorted(stockPrices, key=lambda xy: xy[0])

        num_lines = 0
        curr_line = None
        for i in range(1, len(stockPrices)):
            # We can use y = mx + b because we assume at most one stock price per day
            
            x1, y1 = stockPrices[i]
            if curr_line is None:
                # If there is no line then we need to create (our first) line
                x0, y0 = stockPrices[i-1]
                dx = x1 - x0
                dy = y1 - y0

                curr_line = (dx, dy, x0, y0)
                num_lines += 1
            else:
                dxl, dyl, xl, yl = curr_line
                dxl0 = x1 - xl
                dyl0 = y1 - yl
                # If we are not on the line we need to form a new line with the previous point
                if dxl0 * dyl != dyl0 * dxl:
                    x0, y0 = stockPrices[i-1]
                    dx = x1 - x0
                    dy = y1 - y0

                    curr_line = (dx, dy, x0, y0)
                    num_lines += 1

        return num_lines