// Improved brute force for
// https://leetcode.com/problems/queries-on-number-of-points-inside-a-circle/solutions/2953744/improved-brute-force-in-c/
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

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
    vector<int> countPoints(vector<vector<int>>& points, vector<vector<int>>& queries) {
        #define MAX_X 500
        #define MAX_Y 500
        #define MIN_X 0
        #define MIN_Y 0
        assert(MIN_X == 0 && MIN_Y == 0);

        int num_points_at_cross[MAX_X + 1][MAX_Y + 1];
        unordered_map<vector<int>, int> points_freq;
        for (int x = 0; x <= MAX_X; x++) { for (int y = 0; y < MAX_Y; y++) { num_points_at_cross[x][y] = 0; } }
        for (auto& point : points) {
            int x = point[0]; int y = point[1];
            assert(MIN_X <= x && x <= MAX_X);
            assert(MIN_Y <= y && y <= MAX_Y);

            // Keep a rasterization so we can query the points when we have a lot of repeated points
            // and small-ish circles
            num_points_at_cross[x][y] += 1;

            // Increase point frequences so we can deal with seriously nasty cases if repeated points AND
            // big circles
            if (points_freq.find(point) == points_freq.end()) points_freq[point] = 1;
            else points_freq[point] ++;
        }

        #define MAX_R 500
        #define MIN_R 0
        unordered_map<vector<int>, int> prev_queries;
        vector<int> counts;
        for (vector<int>& query : queries) {
            auto p = prev_queries.find(query);
            if (p != prev_queries.end()) {
                // When we repeat queries a lot just re-use that work
                assert(p->first == query);
                counts.push_back(p->second);
            } else {
                // Based on whether there are a lot of points in general and/or the circle is small,
                // use rasterization
                int xc = query[0]; int yc = query[1]; int r = query[2];
                assert(MIN_X <= xc && xc <= MAX_X);
                assert(MIN_Y <= yc && yc <= MAX_Y);
                assert(MIN_R <= r && r <= MAX_R);

                int contained = 0;

                // There are smarter ways to rasterize than this, but
                // a box is usually not too bad (cache should be good for most of these)
                if (4 * r * r < points_freq.size()) {
                    int left = max(MIN_X, xc - r); int right = min(MAX_X, xc + r);
                    int bot = max(MIN_Y, yc - r); int top = min(MAX_Y, yc + r);
                    for (int x = left; x <= right; x++) {
                        for (int y = bot; y <= top; y++) {
                            if ((xc - x) * (xc - x) + (yc - y) * (yc - y) <= r * r) contained += num_points_at_cross[x][y];
                        }
                    }
                } else {
                    for (auto& p : points_freq) {
                        auto point = p.first;
                        auto count = p.second;
                        int x = point[0]; int y = point[1];
                        if ((xc - x) * (xc - x) + (yc - y) * (yc - y) <= r * r) contained += count;
                    }
                }
                counts.push_back(contained);
                prev_queries[query] = counts[counts.size() - 1];
            }
        }
        return counts;
    }
};