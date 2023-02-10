#include <iostream>
#include <string>
#include <algorithm>
#include "assert.h"

using namespace std;

// Compile with (because it is using lambda)
/*
g++ 1791g2.cc -std=c++17 && cat 1791g2.txt | ./a.out
*/

// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)

// This shit is O(nlogn)
int main() {
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    for (int i = 0; i < num_tests; i++) {
        int n = -1;
        cin >> n;
        assert(n >= 0);

        int c = -1;
        cin >> c;
        assert(c >= 0);

        int unsorted_right_costs[n];
        pair<int, int> left_costs[n];
        pair<int, int> right_costs[n];
        bool used[n];
        for (int j = 0; j < n; j++) {
            int cost = -1;
            cin >> cost;
            assert(0 <= cost && cost <= 1000000000);

            // Cost is depending on the distance to either side
            left_costs[j] = make_pair(j, 1 + j + cost);
            right_costs[j] = make_pair(j, (n - j) + cost);
            unsorted_right_costs[j] = (n - j) + cost;
            used[j] = false;
        }

        sort(left_costs, left_costs + n, [&](pair<int, int>& a, pair<int, int> b) -> bool {
            return a.second < b.second || a.second == b.second && a.first < b.first;
        });
        sort(right_costs, right_costs + n, [&](pair<int, int>& a, pair<int, int> b) -> bool {
            return a.second < b.second || a.second == b.second && a.first < b.first;
        });

        // XXX
        // cout << "LEFT COSTS\n";
        // for (int k = 0; k < n; k++) {
        //     cout << left_costs[k].first << " " << left_costs[k].second << endl;
        // }
        // cout << "RIGHT COSTS\n";
        // for (int k = 0; k < n; k++) {
        //     cout << right_costs[k].first << " " << right_costs[k].second << endl;
        // }
        // XXX

        // Exclusive upper bound
        int len_lo = 0;
        int len_hi = n + 1;
        while (len_lo + 1 < len_hi) {
            for (int k = 0; k < n; k++) { used[k] = false; }

            int len_mid = len_lo + (len_hi - len_lo) / 2;

            assert(len_mid != 0);
            
            // Try to find an optimal combination assuming you can start from either side
            int left_ptr = 0;
            int right_ptr = 0;
            int money = c;
            int max_cost = -1;
            bool used_left = false;
            int used_count = 0;
            while (
                used_count < len_mid &&
                ((left_ptr < n && money - left_costs[left_ptr].second >= 0) ||
                (right_ptr < n && money - right_costs[right_ptr].second >= 0))
            ) {
                if (used[left_costs[left_ptr].first]) { left_ptr ++; }
                else if (used[right_costs[right_ptr].first]) { right_ptr++; }
                else {
                    assert(!(left_ptr >= n && right_ptr >= n));
                    assert(left_ptr >= n || !used[left_costs[left_ptr].first]);
                    assert(right_ptr >=n || !used[right_costs[right_ptr].first]);

                    if (
                        right_ptr >= n || 
                        left_costs[left_ptr].second < right_costs[right_ptr].second
                    ) {
                        int taken_cost = left_costs[left_ptr].second;
                        int taken_idx = left_costs[left_ptr].first;
                        // cout << "\t was able to take " << taken_idx << " from the left\n"; // XXX
                        money -= taken_cost;
                        used[taken_idx] = true;
                        left_ptr++;
                        used_left = true;
                        used_count ++;
                        max_cost = max(max_cost, taken_cost);
                    } else {
                        assert(right_ptr < n);
                        int taken_cost = right_costs[right_ptr].second;
                        int taken_idx = right_costs[right_ptr].first;
                        // cout << "\t was able to take " << taken_idx << " from the left\n"; // XXX
                        money -= taken_cost;
                        used[taken_idx] = true;
                        right_ptr++;
                        used_count ++;
                        max_cost = max(max_cost, taken_cost);
                    }

                    assert(money >= 0);
                }
            }
            assert(used_count <= len_mid);

            bool success = (used_count == len_mid) && used_left;

            // If we did not use the left then we'll have to swap out one of our moves for a left move
            // since you start from the middle. We use max_cost = -1 to signal everything was too expensive
            if (used_count == len_mid && (!used_left && max_cost != -1)) {
                assert(max_cost != -1);
                for (int k = 0; k < n; k++) {
                    int remaining_money = -1;
                    if (used[left_costs[k].first]) { remaining_money = money + unsorted_right_costs[k] - left_costs[k].second; }
                    else { remaining_money = money + max_cost - left_costs[k].second; }

                    if (remaining_money >= 0) {
                        success = true;
                        break;
                    }
                }
            }

            if (success) {
                // cout << "Success with length " << len_mid << endl; // XXX
                len_lo = len_mid;
            }
            else { len_hi = len_mid; }
        }

        assert(0 <= len_lo && len_lo <= n);
        cout << len_lo << endl;
    }
}