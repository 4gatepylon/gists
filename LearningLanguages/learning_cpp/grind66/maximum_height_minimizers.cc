class Solution {
public:
    // Assume that the tree is connected...
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        if (n == 0) return vector<int>();
        if (n == 1) return vector<int>(1, 0);

        // Create a graph that can easily be modified to remove nodes
        vector<unordered_set<int>> g;
        for (int i = 0; i < n; i++) { g.push_back(unordered_set<int>()); }

        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            // DEBUG
            // assert(0 <= u && u <= n);
            // assert(0 <= v && v <= n);

            g[u].insert(v);
            g[v].insert(u);
        }

        // BFS
        queue<int> q;

        // Load the priority queue
        for (int i = 0; i < n; i++) {
            if (g[i].size() == 1) q.push(i);
        }

        // BFS to remove all the leaves in one fell swoop
        // The Queue will only ever contain leaves
        // Leaves can only be formed from leaves that fell
        while (n > 2) {
            // DEBUG
            // assert(q.size() >= 2);

            // Leaf -> Non-Leaf (may be future leaf)
            vector<pair<int, int>> edges2remove;

            // Remove all leaves and look at their edges
            while (q.size() > 0) {
                int r = q.front();
                q.pop();
                // DEBUG
                // assert(g[r].size() == 1);

                // There will only be one of these
                for (int neigh : g[r]) { edges2remove.push_back(pair<int, int>{r, neigh}); }

                // Make sure we remember that we removed this leaf
                n --;
            }
            // Remove all these edges (ignoring from the leaves since we won't go to them
            // again) and add any new leaves for the next iteration
            for (pair<int, int> edge : edges2remove) {
                g[edge.second].erase(edge.first);
                if (g[edge.second].size() == 1) q.push(edge.second);
            }
        }
        
        // DEBUG
        // assert(0 < q.size() && q.size() <= 2); 

        vector<int> sols;
        while (q.size() > 0) {
            sols.push_back(q.front());
            q.pop();
        }
        return sols;
    }
};