#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include "assert.h"

// g++ -std=c++14 A.cc && cat A.txt | ./a.out

using namespace std;

// All prefixes/suffixes are present so here we do this:
// 0. Sort by size
// 1. If all the last characters of each substring of corresponding size are equal (they are all pairs)
//    then it should be a palindrome.

// O(n^2 * T) algo
int main() {
    int T;
    cin >> T;

    // cout << "T: " << T << endl; // XXX

    while (T--) {
        // cout << "T: " << T << endl; // XXX
        // The prefixes/suffixes
        vector<string> strs;

        // Size of the desired string
        int n;
        cin >> n;
        // cout << "n: " << n << endl; // XXX

        assert(n >= 2);

        // Collect them
        for (int i = 0; i < 2 * n - 2; i++) {
            string s;
            cin >> s;
            // cout << "ate " << s << endl; // XXX
            strs.push_back(s);
        }
        sort(strs.begin(), strs.end(), [](string a, string b) -> bool { return a.size() < b.size(); });
        
        assert(strs.size() == 2 * n - 2);
        for (int i = 0; i < strs.size() - 1; i++) { assert(strs[i].size() <= strs[i+1].size()); }
        // for (int i = 0; i < strs.size(); i++) { cout << strs[i] << " "; } // XXX
        // cout << endl; // XXX
        for (int i = 0; i < strs.size(); i += 2) { assert(strs[i].size() == strs[i+1].size()); }

        bool ok = true;
        for (int i = 0; i < strs.size(); i += 2) {
            if (strs[i][strs[i].size() - 1] != strs[i+1][0]) {
                ok = false;
                break;
            } else {
                // Order the next two so that the prefix comes first
                if (i < strs.size() - 2 && strs[i+2].substr(0, strs[i+2].size() - 1) != strs[i]) {
                    // Is it possible to have a string such that both the next prefix and the next 
                    // suffix are prefixed by the current prefix (this needs to be true for anything to break)
                    string temp = strs[i+2];
                    strs[i+2] = strs[i+3];
                    strs[i+3] = temp;
                }
            }
        }


        if (ok) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }

  // Done
  return 0;
}