class Solution {
public:
    int trap(vector<int>& height) {
        int prev_max = 0;
        int water = 0;
        int basket = 0;
        // Global max helps us do less iterations backwards
        int global_max = 0;
        int last_global_max_idx = 0;
        // Forward
        for (int i = 0; i < height.size(); i++) {
            if (height[i] > global_max) global_max = height[i];
            if (height[i] == global_max) last_global_max_idx = i;
            if (height[i] >= prev_max) {
                prev_max = height[i];
                water += basket;
                basket = 0;
            } else if (prev_max > 0) {;
                basket += (prev_max - height[i]);
            }
        }
        // Backward
        prev_max = 0;
        basket = 0;
        for (int i = height.size() - 1; i >= last_global_max_idx; i--) {
            if (height[i] >= prev_max) {
                prev_max = height[i];
                water += basket;
                basket = 0;
            } else if (prev_max > 0) {
                basket += (prev_max - height[i]);
            }
        }
        return water;
    }
};