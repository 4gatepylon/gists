class Solution {
public:
    // Ints can be more negative than positive, so it's OK to use the
    // negative as our key.
    // Simply scan it with a min-heap
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        // Augment with distances
        for (int i = 0; i < points.size(); i++) {
            points[i].push_back(points[i][0]*points[i][0] + points[i][1]*points[i][1]);
        }

        // Priority queue of indices
        // OK to copy by value since it's a reference <- LOL NO
        auto cmp = [&points](const int i, const int j) { return points[i][2] < points[j][2]; };
        // The way C++ seems to work is you need to use this keyword decltype to get the type
        // of cmp, then you pass it into the arguments of the constructor
        auto distIndices = std::priority_queue<int, std::vector<int>, decltype(cmp)>(cmp);

        // Scan the points to find the one with the least index
        for (int i = 0; i < points.size(); i++) {
            // cout << i << "\n";
            if (distIndices.size() < k || points[i][2] < points[distIndices.top()][2]) {
                if (distIndices.size() >= k) {
                    distIndices.pop();
                } 
                distIndices.push(i);
            }
        }

        // Un-augment from distances
        for (int i = 0; i < points.size(); i++) {
            points[i].pop_back();
        }

        // Create the actual points to return
        vector<vector<int>> distPoints;
        while (!distIndices.empty()) {
            int i = distIndices.top();
            distIndices.pop();

            distPoints.push_back(points[i]);
        }


        return distPoints;
    }
};