class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        unordered_map<int, int> u;
        unordered_set<int> ans;
        for (int num : nums) {
            if (u.find(num) == u.end()) {
                u[num] = 1;
            } else {
                u[num] ++;
            }
            if (u[num]*3 > nums.size()) ans.insert(num);
        }
        return vector<int>(ans.begin(), ans.end());
    }
};