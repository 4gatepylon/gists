// https://quantum5.ca/2019/04/05/using-unordered-data-structures-on-std-pair-cpp/
namespace std {
    template<typename X, typename Y>
    struct hash<std::pair<X, Y>> {
        std::size_t operator()(const std::pair<X, Y> &pair) const {
            return std::hash<X>()(pair.first) ^ std::hash<Y>()(pair.second);
        }
    };
}


class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> sols;
        unordered_set<int> prev;
        unordered_set<pair<int, int>> seen;
        for (int i = 0; i < nums.size(); i++) {
            for (int j = i + 1; j < nums.size(); j++) {
                int a = nums[i];
                int b = nums[j];
                int c = -(a + b);
                
                if (prev.count(c) > 0) {
                    // Max and min so that we can have unique hashing of pairs
                    // (xoring more than 2 things scares me, but maybe its not that
                    // logical... at least we save memory... I think because everything
                    // should be iid xoring should work....)
                    int x1, x2;
                    x1 = max(a, max(b, c));
                    x2 = min(a, min(b, c));
                    pair<int, int> t{x1, x2};
                    if (seen.count(t) == 0) {
                        sols.push_back(vector<int>{a, b, c});
                        seen.insert(t);
                    }
                }
            }
            prev.insert(nums[i]);
        }
        return sols;
    }
};