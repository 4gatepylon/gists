#include <algorithm>
// I hate stl
namespace std {
    template<>
    struct hash<std::vector<int>> {
        std::size_t operator()(const std::vector<int> &nums) const {
            // It's ok to put it in zero because we won't be making multiple copies of this
            if (nums.size() == 0) return 0;
            std::size_t h = 0;
            for (int n : nums) {
                h = h ^ std::hash<int>()(n);
            }
            return h;
        }
    };
}
class Solution {
public:
    int maxPoints(vector<vector<int>>& points) {
        int max_points = 1;
        for (int i = 0; i < points.size(); i ++) {
            for (int j = i + 1; j < points.size(); j++) {
                int x0 = points[i][0];
                int y0 = points[i][1];
                int x1 = points[j][0];
                int y1 = points[j][1];

                // Get a simplified fraction
                int dx_check = x1 - x0;
                int dy_check = y1 - y0;
                int g_check = std::gcd(abs(dx_check), abs(dy_check));
                if (g_check != 0) {
                    dx_check /= g_check;
                    dy_check /= g_check;
                }

                int curr_points = 2;
                for (int k = 0; k < points.size(); k++) {
                    if (k != i && k != j) {
                        int x2 = points[k][0];
                        int y2 = points[k][1];
                        int dx = x2 - x0;
                        int dy = y2 - y0;
                        int g = std::gcd(abs(dx), abs(dy));
                        if (g != 0) {
                            dx /= g;
                            dy /= g;
                        }
                        if ((dx == -dx_check && dx != 0)|| (dy == -dy_check && dy != 0)) {
                            dx *= -1;
                            dy *= -1;
                        }
                        if (dx == dx_check && dy == dy_check) curr_points ++;
                    }
                }
                max_points = max(curr_points, max_points);
            }
        }
        return max_points;
    }
};