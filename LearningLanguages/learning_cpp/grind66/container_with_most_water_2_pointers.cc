class Solution {
public:
    // O(n) baby
    int maxArea(vector<int>& height) {
        int i = 0;
        int j = height.size() - 1;
        int best_area = (j - i) * min(height[i], height[j]);
        // There is a problem if height if i and j are the same
        while (i < j) {
            if (height[i] == height[j]) {
                // A little ugly, but find the next two elements between them
                // bigger than them (closest to the two sides) and try both
                // windows ending on each and then go to the middle
                int x = i + 1;
                int y = j - 1;
                while (x < j && height[x] < height[i]) x ++;
                while (y > i && height[y] < height[j]) y --;
                // If there are no larger elements then we cannot improve by narrowing
                if (x >= j) break;
                if (y <= i) break;

                best_area = max(best_area, (y - i) * min(height[i], height[y]));
                best_area = max(best_area, (j - x) * min(height[x], height[j]));
                best_area = max(best_area, (y - x) * min(height[x], height[y]));

                // Skip everything in between because it's obviously not optimal sine
                // i and j would have done better
                i = x;
                y = j;
            } else {
                if (height[i] < height[j]) {
                    i ++;
                } else {
                    j --;
                }
                int area = (j - i) * min(height[i], height[j]);
                best_area = max(best_area, area);
            }
        }
        return best_area;
    }
};