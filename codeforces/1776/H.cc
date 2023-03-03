#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;
// Compile with
/*
g++ H.cc && cat H.txt | ./a.out
*/

int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    for (int i = 0; i < num_tests; i++) {
        int n = -1;
        cin >> n;
        assert(n >= 0);

        int num_moved = n;
        bool moved[n + 1];
        int a[n + 1];
        int b[n + 1];
        for (int j = 1; j < n + 1; j++) { cin >> a[j]; moved[j] = false; }
        for (int j = 1; j < n + 1; j++) { cin >> b[j]; }
        a[0] = -1; b[0] = -1;
        // a -> b
        int jb = n;
        for (int ja = n; ja > 0; ja --) {
            if (moved[b[jb]]) {
                // cout << "ja: " << ja << " jb: " << jb << " with values " << a[ja] << " " << b[jb] << " but " << b[jb] << " at " << jb << " is moved" << endl;
                break;
            }
            else if (a[ja] == b[jb]) { /*cout << "matched at " << ja << " " << jb << endl;*/ jb--; num_moved --; }
            else {
                // cout << "ja: " << ja << " jb: " << jb << " and mismatch with values " << a[ja] << " " << b[jb] << endl;
                // cout << "moving " << a[ja] << endl;
                moved[a[ja]] = true;
            }
        }
        cout << num_moved << endl;
    }
    return 0;
}