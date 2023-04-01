#include <math.h>
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        if (heights.size() == 0) return 0;

        vector<int> left_below(heights.size(), -1);
        vector<int> right_below(heights.size(), heights.size());
        vector<int> stk;
        stk.push_back(-1);
        for (int i = 0; i < heights.size(); i++) {
            while (stk[stk.size() - 1] != -1 && heights[stk[stk.size() - 1]] >= heights[i]) stk.pop_back();
            left_below[i] = stk[stk.size() - 1];
            stk.push_back(i);
        }
        stk.clear();
        stk.push_back(heights.size());
        for (int i = heights.size() - 1; i >= 0; i--) {
            while (stk[stk.size() - 1] != heights.size() && heights[stk[stk.size() - 1]] >= heights[i]) stk.pop_back();
            right_below[i] = stk[stk.size() - 1];
            stk.push_back(i);
        }

        int m = 0;
        for (int i = 0; i < heights.size(); i++) {
            m = max(m, heights[i]*(right_below[i] - left_below[i] - 1));
        }
        return m;
    }
};