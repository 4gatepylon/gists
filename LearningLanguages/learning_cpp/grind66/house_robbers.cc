class Solution {
public:
    int rob(vector<int>& nums) {
        vector<int> robbed(nums.size(), 0);
        vector<int> _not(nums.size(), 0);

        robbed[nums.size() - 1] = nums[nums.size() - 1];
        for (int i = nums.size() - 2; i >= 0; i--) {
            robbed[i] = _not[i+1] + nums[i];
            _not[i] = max(robbed[i+1], _not[i+1]);
        }
        return max(robbed[0], _not[0]);
    }
};