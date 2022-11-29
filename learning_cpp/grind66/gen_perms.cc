class Solution {
private:
    vector<vector<int>> ps;
    void fill_ps(vector<int>& nums, int i) {
        if (i == nums.size()) ps.push_back(vector<int>(nums.begin(), nums.end()));
        else {
            // For each possible value
            for (int j = i; j < nums.size(); j++) {
                // Put it in the front
                int tmp = nums[i];
                nums[i] = nums[j];
                nums[j] = tmp;
                // Fill given that
                fill_ps(nums, i+1);
                // Put it back
                nums[j] = nums[i];
                nums[i] = tmp;
            }
        }
    }
public:
    vector<vector<int>> permute(vector<int>& nums) {
        ps.clear();
        fill_ps(nums, 0);
        return ps;
    }
};