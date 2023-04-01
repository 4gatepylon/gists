#include <numeric>
class Solution {
// O(TN) where there are N elements in nums and T possible values for the sum (T ~ T_avg * N)
// so if we call the average value of each element, T_avg = t, we have ~ O(tN^2), which for 
// N <= 200 and t <= 100 means we have aroudn 4 million max (4 * 10^6) runtime...
private:
    vector<vector<int>> memo;
    bool exists_subset(vector<int>& nums, int i, int target) {
        // Can do this because elements are positive
        if (target < 0) return false;
        else if (target == 0) return true;
        else if (i >= nums.size()) return false;
        else if (memo[i][target] != -1) return memo[i][target] != 0;

        bool ans = exists_subset(nums, i + 1, target) || exists_subset(nums, i + 1, target - nums[i]);
        memo[i][target] = ans ? 1 : 0;
        return ans;
    }
public:
    bool canPartition(vector<int>& nums) {
        int sum = std::accumulate(nums.begin(), nums.end(), 0);
        if (sum % 2 == 1) return false;
        // If we can find a subset of nums with a sum of sum / 2 we will know that
        // the remainder has that size too and so we have solved the problem
        memo = vector<vector<int>>(nums.size(), vector<int>(sum / 2 + 1, -1));
        return exists_subset(nums, 0, sum / 2);
    }
};