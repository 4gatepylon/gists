class Solution {
private:
    int leftmost_above(vector<int> vals, vector<int> indices, int val) {
        int lo = -1;
        int hi = indices.size() - 1;
        while (lo + 1 < hi) {
            int mid = hi - (hi - lo) / 2;
            if (vals[indices[mid]] >= val) hi = mid;
            else lo = mid;
        }
        if (vals[indices[hi]] >= val) return indices[hi];
        return -1;
    }
public:
    int maxArea(vector<int>& height) {
        vector<int> incr;
        vector<vector<int>> bigger(2, vector<int>(height.size(), -1));
        
        for (int k = 0; k < 2; k ++) {
            int dx = k == 0 ? 1 : -1;
            int x0 = k == 0 ? 0 : height.size() - 1;
            incr.clear();
            for (int i = x0; 0 <= i && i < height.size(); i += dx) {
                // Find the earliest element larger than this one
                // (using itself causes zero width)
                bigger[k][i] = i;
                if (incr.size() > 0) {
                    int idx = leftmost_above(height, incr, height[i]);
                    if (idx != -1) bigger[k][i] = idx;
                }
                // Update that set of possible elements
                if (incr.size() == 0 || height[i] > height[incr[incr.size() - 1]]) {
                    incr.push_back(i);
                }
            }
        }
        int max_water = 0;
        for (int i = 0; i < height.size(); i++) {
            for (int k = 0; k < 2; k++) {
                int boundary = bigger[k][i];
                int dist = boundary > i ? boundary - i : i - boundary;
                int area = dist * height[i];
                if (area > max_water) max_water = area;
            }
        }
        return max_water;
    }
};