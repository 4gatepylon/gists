#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include "assert.h"

// g++ -std=c++14 c.cc && cat c.txt | ./a.out

using namespace std;

int main() {
    int T;
    cin >> T;

    // cout << "T: " << T << endl; // XXX

    while (T--) {
        // cout << "T: " << T << endl; // XXX
        int n;
        cin >> n;
        // cout << "n: " << n << endl; // XXX

        int a[n];
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        // starts is inclusive
        int starts[n];
        int lengths[n];
        
        starts[0] = 0;
        lengths[0] = 1;
        for (int i = 1; i < n; i++) {
            starts[i] = starts[i-1];
            lengths[i] = lengths[i-1];

            // TODO
            // You should pop off from the left if the right element is strictly greater
            // Otherwise you should consider just adding it if it is greater than the previous length
            // After that, you should only consider starting anew (from this element), which you should only do
            // if it is greater than the score of the previous option(s)
            if (a[i] >= lengths[i-1]) {
                lengths[i]++;
            }
        }

        // Give the answer
        for (int i = 0; i < n - 1; i++) {
            cout << lengths[i] << " ";
        }
        if (n >= 1) cout << lengths[n-1] << endl;
    }

  // Done
  return 0;
}