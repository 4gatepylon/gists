#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;
// Compile with
/*
g++ A.cc && cat A.txt | ./a.out
*/

int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    for (int i = 0; i < num_tests; i++) {
        int n = -1;
        cin >> n;
        assert(n >= 1);
        int curr_time = 0;
        int walk_times = 0;
        // cout << "n is " << n << endl; // XXX
        for (int j = 0; j < n + 1; j++) {            
            int next_time = -1;
            if (j < n) cin >> next_time;
            else              next_time = 1440;
            // cout << "next time " << next_time << endl; // XXX
            assert(next_time >= 0);
            if (next_time - curr_time >= 2 * 120) { walk_times += 2;
                // cout << "walked between " << curr_time << " and " << next_time << endl; 
            }
            else if (next_time - curr_time >= 120) { walk_times++;
                // cout << "walked between " << curr_time << " and " << next_time << endl; 
            }
            curr_time = next_time;
        }
        if (walk_times >= 2) { cout << "YES" << endl; }
        else { cout << "NO" << endl; }
    }
    return 0;
}