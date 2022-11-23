class Solution {
public:
    // This is basically shortest path to any rotten orange from
    // every non-rotten orange. If the graph is disconnected then -1.
    // We can do this using BFS basically simulating the rotting.
    // We can store 2 + m where m will be the number of minutes.
    int orangesRotting(vector<vector<int>>& grid) {
        if (grid.size() == 0) {
            return -1;
        }

        queue<int> is;
        queue<int> js;
        queue<int> dists;
        // Change so that it's -1 for 0 and 10000 for 1 and 0 for 2's
        #define BIG 10000
        #define EMPTY -1
        // Init Graph basically so that we can just take a min size or whatever
        bool found_fresh = false;
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[0].size(); j++) {
                if (grid[i][j] == 2) {
                    grid[i][j] = BIG;
                    is.push(i);
                    js.push(j);
                    dists.push(-1);
                } else if (grid[i][j] == 1) {
                    grid[i][j] = BIG;
                    found_fresh = true;
                } else  {
                    grid[i][j] = EMPTY;
                }
            }
        }

        // If there are no fresh oranges then we will have rotted in the first minute
        if (!found_fresh) {
            return 0;
        }

        // js won't be empty until is is empty then they're both empty
        // BFS
        while (!is.empty()) {
            // Get the next element to visit
            int i = is.front();
            int j = js.front();
            int dist = dists.front() + 1;
            
            is.pop();
            js.pop();
            dists.pop();

            // Don't update if already visited because BFS is shortest path
            // (weights are one)
            if (grid[i][j] == BIG) {
                // Update it
                grid[i][j] = dist;

                // See who else we should visit
                for (int k = 0; k < 2; k++) {
                    for (int d = -1; d < 2; d++) {
                        int ip = k == 0 ? i + d : i;
                        int jp = k == 1 ? j + d : j;
                        if (0 <= ip && ip < grid.size() && 0 <= jp && jp < grid[0].size() && grid[ip][jp] == BIG) {
                            is.push(ip);
                            js.push(jp);
                            dists.push(dist);
                        }
                    }
                }
            }
        }

        // Get the last element
        int m = 0;
        for (int i = 0; i < grid.size(); i++) { m = max(m, *std::max_element(grid[i].begin(), grid[i].end())); }
        return m == BIG ? -1 : m;
    }
};