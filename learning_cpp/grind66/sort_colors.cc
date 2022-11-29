class Solution {
public:
    void swp(vector<int>& nums, int i, int j) {
        int tmp = nums[j];
        nums[j] = nums[i];
        nums[i] = tmp;
    }
    void sortColors(vector<int>& nums) {
        int i = 0;
        int j = nums.size() - 1;
        if (nums[j] != 2 && nums[i] == 2) swp(nums, i, j);
        if (nums[i] != 0 && nums[j] == 0) swp(nums, i, j);
        if (nums[i] == 0) i ++;
        if (nums[j] == 2) j --;
        int x = i;
        int y = j;
        // 1 2 1
        while (x <= y) {
            // Put anything where it belongs
            if (nums[x] == 0) swp(nums, i, x);
            if (nums[x] == 2) swp(nums, j, x);
            if (nums[y] == 0) swp(nums, i, y);
            if (nums[y] == 2) swp(nums, j, y);
            // Update i and j
            if (nums[i] == 0) i ++;
            if (nums[j] == 2) j --;
            // Update x and y s.t. if we have something that should go to an i or j
            // leave it to be fixed in the next iteration (otherwise, move on)
            if (nums[x] != 0 && nums[x] != 2 || x < i) x ++;
            if (nums[y] != 2 && nums[y] != 0 || y > j) y --;
            
        }
    }
};