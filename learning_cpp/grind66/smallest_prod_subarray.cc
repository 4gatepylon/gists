class Solution {
public:
    // Isn't this the same as sum? Isn't any such reducer the same?
    int maxProduct(vector<int>& nums) {
        if(nums.size() == 0) return 0;
        vector<int> largest_product_at(nums.size(), 0);
        vector<int> smallest_product_at(nums.size(), 0);
        largest_product_at[nums.size() - 1] = nums[nums.size() - 1];
        smallest_product_at[nums.size() - 1] = nums[nums.size() - 1];
        for (int i = nums.size() - 2; i >= 0; i--) {
            // The largest product is either the most negative or the most positive or just the number
            int pl = largest_product_at[i + 1] * nums[i];
            int ps = smallest_product_at[i + 1] * nums[i];
            // Update largest product
            if (pl >= ps && pl >= nums[i]) largest_product_at[i] = pl;
            else if (ps >= pl && ps >= nums[i]) largest_product_at[i] = ps;
            else largest_product_at[i] = nums[i];
            // Update the smallest
            if (pl <= ps && pl <= nums[i]) smallest_product_at[i] = pl;
            else if (ps <= pl && ps <= nums[i]) smallest_product_at[i] = ps;
            else smallest_product_at[i] = nums[i];
        }
        return *max_element(largest_product_at.begin(), largest_product_at.end());
    }
};