class Solution {
public:
    int maxArea(vector<int>& height) {
        // DP
        // Each container can be mapped to the lower of the two elements
        // For each container we can find the furthest element to the left
        // (and to the right) s.t. it is larger than our element
        // We can do that with a 2-sided queue
        //   - If we are larger than the right side (stack) then we pop (right) until
        //     we aren't
        //   - Then return either leftmost element (queue)
        // Init with -1 for ease of use
        deque<int> traverser;
        vector<vector<int>> bigger(2, vector<int>(height.size(), -1));
        
        for (int k = 0; k < 2; k ++) {
            int dx = k == 0 ? 1 : -1;
            int x0 = k == 0 ? 0 : height.size() - 1;
            int boundary_x = x0 - dx;
            traverser.clear();
            traverser.push_back(boundary_x);
            // assert(traverser.size() == 1); // DEBUG
            // assert(traverser.back() == traverser.front() && traverser.front() == boundary_x); // DEBUG
            for (int i = x0; 0 <= i && i < height.size(); i += dx) {
                while (traverser.back() != boundary_x && traverser.back() < height[i]) traverser.pop_back();
                // assert(traverser.size() >= 1); // DEBUG
                // NOTE: we want to avoid using -1 since it's not an actual value...
                cout << "----\nTRAVERSER("<<i<<"SIZE " << traverser.size() << "\n";
                for (int z = 0; z < traverser.size(); z ++) {cout << " " << traverser[z];}
                cout << "\n";
                bigger[k][i] = traverser.size() == 1 ? traverser.front() : traverser[1];
                traverser.push_back(height[i]);
            }
        }
        int max_water = 0;
        for (int i = 0; i < height.size(); i++) {
            for (int k = 0; k < 2; k++) {
                int boundary = bigger[k][i];
                int dist = boundary > i ? boundary - i : i - boundary;
                int area = dist * height[i];
                cout << "-----\n";
                cout << (k == 0 ? "left\n" : "right\n");
                cout << area << " " << boundary << " " << i << "\n";
                if (0 <= boundary && boundary < height.size() && area > max_water) max_water = area;
            }
        }
        return max_water;
    }
};