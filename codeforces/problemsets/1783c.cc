#include <iostream>
#include <string>
#include <algorithm>
#include "assert.h"

// https://codeforces.com/problemset/problem/1783/C

using namespace std;

// Compile with
/*
g++ 1783c.cc && cat 1783c.txt | ./a.out
*/

// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
int main() {
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    for (int i = 0; i < num_tests; i++) {
        int n = -1, m = -1;
        cin >> n >> m;
        assert(n >= 1 && m >= 0);

        int costs[n];        // Used to see how much it would cost to get wins for the people we want to beat
        int sorted_costs[n]; // Used to greedily get the cheapest wins possible
        for (int j = 0; j < n; j++) {
            cin >> costs[j];
            assert(costs[j] >= 0);

            sorted_costs[j] = costs[j];
        }
        sort(sorted_costs, sorted_costs + n); // Actually we could optimize this with a histo since 0 <= ai <= 1000

        // If you win exactly zero and lose everything: always possible
        int min_place = n + 1;
        
        int best_num_wins_lo = 0;     // Beating no one (inclusive)
        int best_num_wins_hi = n + 1; // Beating everyone (not inclusive)
        while (best_num_wins_hi - best_num_wins_lo > 1) {
            // Assume we tie with this mid person, but they may or may not beat us
            // in the former case we get +1 placement (bad)
            int best_num_wins_mid = (best_num_wins_lo + best_num_wins_hi) / 2;
            
            // Greedily take wins and then try to beat them, swapping with the most expensive win we have
            int bank = m;
            for (int k = 0; k < best_num_wins_mid; k++) { bank -= sorted_costs[k]; }
            if (bank >= 0) {
                // We can win this many wins
                best_num_wins_lo = best_num_wins_mid;
                if (best_num_wins_mid == n) {
                    min_place = 1;
                    break;
                } 
                // Else

                // Assume he's going to be tied in placement with the current tier in wins
                int placement = n - best_num_wins_mid;
                assert(n >= placement && placement >= 1);

                // cout << "placement to begin with: " << n << " - " << best_num_wins_mid << " = " << placement << endl;
                // You should always win at least 0, but if you win all then you can't go up in placement
                assert(best_num_wins_mid >= 0);
                if (placement >= 1 && bank + sorted_costs[best_num_wins_mid - 1] < costs[best_num_wins_mid]) {
                    // If our leftover money and that gotten by giving away the most expensive win we have
                    // we can beat the person we tied with, then we don't get +1 placement
                    // cout << "bank: " << bank << " sorted_costs[best_num_wins_mid]: " << sorted_costs[best_num_wins_mid] << " costs[best_num_wins_mid]: " << costs[best_num_wins_mid] << endl;
                    placement += 1;
                }
                // cout << "was able to get " << best_num_wins_mid << " wins, placing " << placement << " with bank " << bank << endl;
                min_place = min(min_place, placement);
                
            } else {
                // We can't win this many wins
                best_num_wins_hi = best_num_wins_mid;
            }
        }
        // There is no need to re-write min_place because either we hit the else above in which case the
        // latest mid was already tested and then the things above it were impossible (or it was always impossible
        // and then n + 1 is actually correct), or we did not hit the else (went into if (bank >= 0)) in which case
        // we set the min place immediately

        cout << min_place << endl;
    }
}