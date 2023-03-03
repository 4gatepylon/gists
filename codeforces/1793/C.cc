#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;
// Compile with
/*
g++ C.cc && cat C.txt | ./a.out
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
        assert(n >= 0);

        int a[n+1];
        a[0] = -1;
        for (int j = 1; j < n + 1; j++) { cin >> a[j]; }
        int lo_val = 1;
        int lo_idx = 1;
        int hi_val = n;
        int hi_idx = n;
        while (lo_idx < hi_idx) {
            // cout << lo_idx << " " << hi_idx << endl;
            if (a[lo_idx] == lo_val) {
                lo_val ++;
                lo_idx ++;
            } else if (a[lo_idx] == hi_val) {
                hi_val --;
                lo_idx ++;
            } else if (a[hi_idx] == lo_val) {
                lo_val ++;
                hi_idx --;
            } else if (a[hi_idx] == hi_val) {
                hi_val --;
                hi_idx --;
            } else {
                break;
            }
        }
        // cout << "***" << endl;
        if (lo_idx != hi_idx) { cout << lo_idx << " " << hi_idx << endl; }
        else { cout << -1 << endl; }

        // To find such a subsegment we require that it have length at least 4 because the minimum has to be in the middle,
        // and the maximum also has to be in the middle (they cannot be identical) and we must have two ends

        // O(n^2) solution: for any two indexes (not necessarily next to eachother) see if there is an element to the left
        // and an element to the right s.t. those elements are between them


    }
    return 0;
}