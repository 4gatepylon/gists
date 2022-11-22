class Solution {
public:
    // This is Cycle Detection
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        // Create a Graph and it's Graph Transpose
        // NOTE we will be mutating pointedBy
        vector<unordered_set<int>> pointsTo(numCourses);
        vector<unordered_set<int>> pointedBy(numCourses);
        int numEdges = 0;
        for (vector<int> inducing : prerequisites) {
            numEdges ++;

            // Inducer -> Induced
            int induced = inducing[0];
            int inducer = inducing[1];

            // Inducer points to them
            pointsTo[inducer].insert(induced);
            pointedBy[induced].insert(inducer);
        }

        // We will use Kahn's algorithm which will detect cycles
        // https://en.wikipedia.org/wiki/Topological_sorting
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (pointedBy[i].size() == 0) {
                q.push(i);
            }
        }

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            for (int next : pointsTo[curr]) {
                pointedBy[next].erase(curr);
                numEdges --;
                if (pointedBy[next].size() == 0) {
                    q.push(next);
                }
            }
        }

        return numEdges == 0;
    }
};