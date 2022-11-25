#include <math.h>
class Solution {
public:
    #define NO_SOL 10001
    int coinChange(vector<int>& coins, int amount) {
        // Fewest amt(X) = min(Fewest amt(X - i)) over i being the values of our coins
        // If we have I coins with min value i, then an upper bound on our runtime is O(I(V/i))
        // Since there are at most 12 coins this is basically O(V) where V is the total amount
        unordered_map<int, int> amt2min;
        int ans = cc(coins, amount, amt2min);
        return ans == NO_SOL ? -1 : ans;
    }
    // Because amount is <= 10^4, return 10^4 + 1 = 10001 if there is no solution
    // (or just generally above 10^4)
    int cc(vector<int>& coins, int amt, unordered_map<int, int>& amt2min) {
        if (amt == 0) {
            return 0;
        } else if (amt < 0) {
            return NO_SOL;
        } else if (amt2min.count(amt) > 0) {
            return amt2min[amt];
        }
        int sol = NO_SOL;
        for (int coin : coins) {
            sol = min(sol, 1 + cc(coins, amt - coin, amt2min));
        }
        amt2min[amt] = sol;
        return sol;
    }
};