#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include "assert.h"

// g++ B.cc && cat B.txt | ./a.out

using namespace std;

// All prefixes/suffixes are present so here we do this:
// 0. Sort by size
// 1. If all the last characters of each substring of corresponding size are equal (they are all pairs)
//    then it should be a palindrome.

int main() {
    int T;
    cin >> T;

    // cout << "T: " << T << endl; // XXX

    while (T--) {
        // Size of the desired string
        int n;
        cin >> n;
        assert(1 <= n && n <= 50000);
        int nums[n];
        for (int i = 0; i < n; i++) {
            cin >> nums[i];
            assert(1 <= nums[i] && nums[i] <= 1000000000);
        }
        for (int i = 0; i < n; i++) {
            if (nums[i] == 1) { nums[i] ++; }
            if (i >= 1 && nums[i] >= nums[i-1] && nums[i] % nums[i-1] == 0) { nums[i] ++; }
        }

        for (int i = 0; i < n - 1; i++) {
            cout << nums[i] << " ";
        }
        if (n >= 1) cout << nums[n-1] << endl;

        for (int i = 0; i < n; i++) { assert(nums[i] != 1); }
        for (int i = 1; i < n; i++) { assert(nums[i] % nums[i-1] != 0); }
    }

  // Done
  return 0;
}