class Solution {
private:
    int area(vector<int>& h, int height) {
        bool read = false;
        int w_max = 0;
        int w = 0;
        for (int i = 0; i < h.size(); i++) {
            if (read) {
                if (h[i] < height) {
                    read = false;
                    w_max = max(w_max, w);
                } else w += h[i];
            } else {
                if (h[i] >= height) {
                    read = true;
                    w = h[i];
                }
            }
        }
        return w_max;
    }
public:
    int maxArea(vector<int>& height) {
        // BS the height and not the heights array's values because
        // we have smaller height values that numbers of elements in the
        // worst case
        int lo = 0;
        int hi = max_element(height.begin(), height.end()) - height.begin();
        int best = 0;
        while (lo < hi - 1) {
            int mid = lo + (hi - lo) / 2;
            int a = area(height, h[mid]);
            if (a >= best) {
                best = a;
                lo = 
            }
        }
        return best;
    }
};