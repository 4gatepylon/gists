#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cassert>

using namespace std;
// Compile with
/*
g++ D.cc && cat D.txt | ./a.out
*/

int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    // For each test find the size of the matrix, then find the most beautiful matrix of that
    // size of a permutation of the numbers 1 to n^2
    for (int i = 0; i < num_tests; i++) {
        int n = -1;
        cin >> n;
        assert(n != -1);
        
        // Values and where they point to (our graph basically)
        vector<int> graph;

        // Zero if it's safe and otherwise the loop its in
        // (-1 for don't know yet)
        vector<int> type;
        for (int j = 0; j < n; j++) { type.push_back(-1); }

        // Populate the graph and begin to do type finding
        queue<int> bfs_starter;
        queue<int> bfs_wavefront;
        for (int j = 0; j < n; j++) {
            int val = -1;
            cin >> val;
            assert(val != -1);
            graph.push_back(j + val);
            if (val + j < 0 || val + j >= n) { type[j] = 0; } // Not in a loop, we know the type
            else { bfs_starter.push(j); }                     // Might be in a loop, we want to find its type
        }

        // Find everyone's type
        int next_type = 1;
        // TODO

        // Make sure we did OK
        assert(type.size() == n && graph.size() == n);
        for (int j = 0; j < n; j++) { type[j] != -1; }

        // Find the number of types and how many per type
        // (exclude type zero because we only care about loops)
        unordered_map<int, int> type_sizes;
        for (int j = 0; j < n; j++) {
            if (type[j] != 0) {
                if (type_sizes.find(j) == type_sizes.end()) { type_sizes[j] = 1; }
                else { type_sizes[j]++; }
            }
        }

        // There is no way to win if there are too many loops to fix all in one move
        if (type_sizes.size() > 1) { cout << 0 << endl; } else {
            // Acount for the zero-type
            // (note we will now have at most two types: the zero type and one loop,
            // just the zero type, or just a loop)
            type_sizes[0] = 0;
            for (int j = 0; j < n; j++) { if (type[j] == 0) { type_sizes[0] ++; } }

            // There is no way to win if there is a single loop only because we can't point out
            // of the loop
            if (type_sizes.size() == 1 && type_sizes.find(0) == type_sizes.end()) { cout << 0 << endl; }

            // The number of ways to remove loops is the number of ways to break a loop
            // if we have a loop, and otherwise the number of ways to point to anyone not pointing to you
            if (type_sizes.size() == 1) {
                assert(type_sizes.find(0) != type_sizes.end());
                // We have one type and it's the zero type, we must count the number of places to point to
                // such that that place is not pointing to us eventually
                // TODO not sure how to do!
            } else {
                assert(type_sizes.size() == 2);
                // We have one loop (type one) and we need to break any one of its elements by pointing to
                // any element in type zero
                assert(type_sizes.find(0) != type_sizes.end());
                assert(type_sizes.find(1) != type_sizes.end());
                cout << type_sizes[0] * type_sizes[1] << endl;
            }
        }
    }
    return 0;
}