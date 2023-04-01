class Solution {
public:
    void print_arr(vector<int>& nums) {
        for (int c : nums) { cout << c << " "; }
        cout << "\n";
    }
    int findDuplicate(vector<int>& nums) {
        int i = 0;
        while (i < nums.size()) {
            if (i + 1 != nums[i]) {
                int j = nums[i] - 1;
                while (nums[i] != i + 1) {
                    if (nums[j] == nums[i]) return nums[j];
                    int tmp = nums[j];
                    // Fix j
                    nums[j] = nums[i];
                    // Try to fix i or continue on the cycle
                    nums[i] = tmp;
                    j = nums[i] - 1;
                }
            }
            i++;
        }
        return -1;
    }
};