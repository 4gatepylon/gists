#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include "assert.h"

using namespace std;

/*
g++ D.cc && cat D.txt | ./a.out
*/

// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    // For each test find the size of the matrix, then find the most beautiful matrix of that
    // size of a permutation of the numbers 1 to n^2
    for (int i = 0; i < num_tests; i++) {
        // Get the dimensions of the graph and the sets of tokens and bonuses
        int num_edges = -1, num_vertices = -1;
        cin >> num_edges; cin >> num_vertices;
        assert(num_edges >= 1); assert(num_vertices >= 1);

        // To easily index
        num_vertices ++;

        int num_tokens = -1, num_bonuses = -1;
        cin >> num_tokens; cin >> num_bonuses;
        assert(num_tokens >= 0); assert(num_bonuses >= 0);

        // Load tokens and bonuses
        int bonuses[num_vertices];
        int tokens[num_vertices];
        for (int j = 0; j < num_vertices; j++) { bonuses[j] = false; tokens[j] = false; }

        for (int j = 0; j < num_tokens; j++) {
            int token = -1;
            cin >> token;
            assert(token >= 0);

            tokens[token] = true;
        }
        
        // Easy case
        if (tokens[1]) {
            cout << "YES" << endl;
        } else {
            //We do not automatically win, so we'll need to figure it out...

            for (int j = 0; j < num_bonuses; j++) {
                int bonus = -1;
                cin >> bonus;
                assert(bonus >= 0);

                bonuses[bonus] = true;
            }

            // Load the edges to create the graph
            vector<vector<int>> graph(num_vertices, vector<int>(0));
            for (int j = 0; j < num_edges; j++) {
                int u = -1, v = -1;
                cin >> u; cin >> v;
                assert(u >= 0); assert(v >= 0);
                graph[u].push_back(v);
                graph[v].push_back(u);
            }

            // Perform BFS to find shortest path through bonuses to get to tokens
            int visited_len[num_vertices];
            int visitor[num_vertices];
            for (int j = 0; j < num_vertices; j++) { visited_len[j] = -1; visitor[j] = -1;}

            queue<int> wavefront;
            for (int j = 0; j < num_vertices; j++) { 
                if (tokens[j]) { 
                    // We have aleady visited the tokens
                    visited_len[j] == 0;
                    visitor[j] = j;
                    // Now go to their neighbors
                    for (int jj= 0; jj < graph[j].size(); jj++) {
                        int v = graph[j][jj];
                        if (bonuses[v] || v == 1) {
                            wavefront.push(v);
                            visitor[v] = j;
                        }
                    }
                }
            }

            // TODO this is a sufficient condition BUT NOT A NECESASARY CONDITION
            // because we can have many short paths and many many tokens and then we can
            // still make it... we can also make it if we have a loop of bonuses in addition to our path...
            // RAN OUT OF TIME

            int win_len = -1;
            int win_visitor = -1;
            while (!wavefront.empty()) {
                int u = wavefront.front();
                wavefront.pop();

                assert(bonuses[u]);
                assert(visited_len[u] == -1);
                assert(visitor[u] != -1);

                if (u == 1) {
                    win_len = visited_len[visitor[u]] + 1;
                    win_visitor = visitor[u];
                    break;
                } else {
                    // visit
                    visited_len[u] = visited_len[visitor[u]] + 1;

                    // u -> v
                    for (int jj = 0; jj < graph[u].size(); jj++) {
                        int v = graph[u][jj];
                        if (visited_len[v] == -1 && (bonuses[v] || v == 1)) {
                            visited_len[v] = visited_len[visitor[u]] + 1;
                            visitor[v] = visitor[u];
                            wavefront.push(v);
                        }
                    }
                }
            }

            // BFS is over, see who won...
            if (win_len == -1) {
                assert(win_visitor == -1);
                cout << "NO" << endl;
            } else {
                assert(win_visitor != -1);

                // Clear this, we'll now use it as a boolean mask to know if we visited
                for (int j = 0; j < num_vertices; j++) { visited_len[j] = -1; }

                // Now search for any equal or longer path, excluding the same starting point...
                // empty the queue and do the same thing again but exluding the starting point
                wavefront = queue<int>();
            }
        }
    }
    return 0;
}