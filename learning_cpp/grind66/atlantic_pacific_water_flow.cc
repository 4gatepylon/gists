class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        // Who we visited and who we can reach from which ocean
        vector<vector<vector<bool>>> 
            masks_visited(2, vector<vector<bool>>(heights.size(), vector<bool>(heights[0].size(), false)));
        vector<vector<vector<bool>>> 
            masks(2, vector<vector<bool>>(heights.size(), vector<bool>(heights[0].size(), false)));
        // The edges to the pacific or atlantic
        vector<int> starting_i {0, (int)heights.size() - 1};
        vector<int> starting_j {0, (int)heights[0].size() - 1};

        // Each k corresponds to pacific or atlantic
        for (int k = 0; k < 2; k++) {
            // Initialize queue's for BFS (starting on a set of points next to the pacific or atlantic)
            queue<int> is;
            queue<int> js;
            // Will duplicate but that's OK!
            for (int i = 0; i < heights.size(); i ++) {
                is.push(i);
                js.push(starting_j[k]);
            }
            for (int j = 0; j < heights[0].size(); j++) {
                is.push(starting_i[k]);
                js.push(j);
            }

            // BFS
            while (!is.empty()) {
                // Get the next point to visit (we know this point is reachable from the corresponding ocean)
                int i = is.front();
                int j = js.front();
                is.pop();
                js.pop();

                // If we didn't visit it already, make sure to visit it
                // and then mark it as reachable from the ocean we are in (k)
                if (!masks_visited[k][i][j]) {
                    masks[k][i][j] = true;
                    masks_visited[k][i][j] = true;
                    for (int d = -1; d < 2; d += 2) {
                        for (int dim = 0; dim < 2; dim++) {
                            int ip = dim == 0 ? i + d : i;
                            int jp = dim == 1 ? j + d : j;
                            if (
                                // Has to be a point on the island
                                0 <= ip && ip < heights.size() && 
                                0 <= jp && jp < heights[0].size() &&
                                // Has to not be visited (we don't visit twice)
                                !masks_visited[k][ip][jp] &&
                                // Has to be a point that has water that could flow
                                // to where we are (because we are going UP the DAG
                                // that goes to the ocean)
                                heights[ip][jp] >= heights[i][j]
                            ) {
                                is.push(ip);
                                js.push(jp);
                            }
                        }
                    }
                }
            }
        }

        
        // Fill coords and return
        vector<vector<int>> coords;
        for (int i = 0; i < heights.size(); i++) {
            for (int j = 0; j < heights[0].size(); j++) {
                if (masks[0][i][j] && masks[1][i][j]) {
                    coords.push_back(vector<int>{i, j});
                }
            }
        }
        return coords;
    }
};