#include <math.h>

class Solution {
public:
    // Note that we will mutate mat
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        // Think of this as a graph
        // Have a queue of indices
        // First look at all indices that are non-zero but touching zero
        // Then for each make sure to append to the queue any index that hasn't
        // yet been looked at
        // We'll set them to -1 for infinity and we'll know they haven't been looked at that way
        // This is O(nm) since we look at each index once
        // Just think BFS where the initial configuration is a front not just a node

        // Queue in the form of two vectors
        queue<int> is;
        queue<int> js;
        for (int i = 0; i < mat.size(); i++) {
            for (int j = 0; j < mat[0].size(); j++) {
                if (mat[i][j] == 1) {
                    // If it's touching a zero then add it to our queue
                    if (
                        (i > 0 && mat[i-1][j] == 0) || 
                        (i < mat.size() - 1 && mat[i+1][j] == 0) ||
                        (j > 0 && mat[i][j-1] == 0) ||
                        (j < mat[0].size() - 1 && mat[i][j+1] == 0)
                    ) {
                        is.push(i);
                        js.push(j);
                    }
                    // Set all of these to infinity to begin with
                    mat[i][j] = -1;
                }
            }
        }
        // Note js should always be empty/full when is is empty/full
        while (!is.empty()) {
            int i = is.front();
            int j = js.front();
            // DEBUG
            // cout << "i, j: " << i << ", "<< j << "\n";
            is.pop();
            js.pop();

            // For all valid neighbors, if it is going to be set in the next wave
            // (BFS) then add it to the queue, otherwise try to get its value into
            // this one (with a +1) if its the smallest
            for (int k = 0; k < 2; k ++) {
                for (int d = -1; d < 2; d += 2) {
                    int ip = k == 1 ? i : i + d;
                    int jp = k == 0 ? j : j + d;
                    if (0 <= ip && ip < mat.size() && 0 <= jp && jp < mat[0].size()) {
                        // DEBUG
                        // cout << "ip, jp: " << ip << ", " << jp << "\n";
                        if (mat[ip][jp] == -1) {
                            is.push(ip);
                            js.push(jp);
                        } else {
                            mat[i][j] = mat[i][j] == -1 ? mat[ip][jp] + 1 : min(mat[i][j], mat[ip][jp] + 1);
                        }
                    }
                }
            }
        }
        // Note by now there should be no elements of the matrix that are negative one
        return mat;
    }
};