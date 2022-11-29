class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        // pivot is the last element
        // such that the seq. is monotone
        int pivot = -1;
        int extract = -1;
        int i;
        for (i = 0; i < nums.size() - 1; i++) {
            if (nums[i] < nums[i+1]) {
                pivot = i;
                extract = i+1;
            } else if (pivot != -1 && nums[i+1] > nums[pivot] && nums[i+1] <= nums[extract]) extract = i+1;
        }
        if (pivot == -1) reverse(nums.begin(), nums.end());
        else {
            int p = nums[pivot];
            nums[pivot] = nums[extract];
            nums[extract] = p;
            reverse(nums.begin() + pivot + 1, nums.end());
        }
    }
};