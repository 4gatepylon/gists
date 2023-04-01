#include <iostream>
#include <string>
#include <vector>
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
        int n = -1;
        cin >> n;
        assert(n >= 1);
        
        int elems[n];
        for (int k = 0; k < n; k++) { cin >> elems[k]; assert(elems[k] >= 0); }

        sort(elems, elems + n);
        vector<int> freqs;
        freqs.push_back(1);
        
        for (int i = 1; i < n; i++) {
            if (elems[i] == elems[i - 1]) {
                freqs[freqs.size() - 1]++;
            } else {
                if (elems[i] - elems[i - 1] > 1) { freqs.push_back(0); }
                freqs.push_back(1);
            }
        }

        // Maximal overlap in any place where there is overlap over the partitioning of disjoint sets of sets
        int count = 0;
        int overlap = 0;
        for (int j = 0; j < freqs.size(); j++) {
            if (freqs[j] > overlap) {
                assert(freqs[j] - overlap > 0);
                count += freqs[j] - overlap;
            } 
            overlap = freqs[j];
        }
        cout << count << endl;

    }
    return 0;
}