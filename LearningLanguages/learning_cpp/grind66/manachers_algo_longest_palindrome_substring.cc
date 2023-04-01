#include <math.h>
#undef DEBUG
class Solution {
private:
    void print_string(vector<char> s) {
        for (int i = 0; i < s.size(); i ++) {cout << s[i];}; cout << "\n";
    }
    void print_dp(vector<int> dp) {
        for (int i = 0; i < dp.size(); i ++) {cout << " " << dp[i];}; cout << "\n";
    }
public:
    string longestPalindrome(string _s) {
        #define SPECIAL '_'
        // We will use Manacher's DP Algorithm
        vector<char> s(2 * _s.size() + 1);
        vector<int> dp(2 * _s.size() + 1);

        // First copy the string and make a version that guarantees that all palindromes
        // will be odd-sized. Makes life easier. We use spaces (SPECIAL) since this is only
        // lower case english letters
        s[0] = SPECIAL;
        dp[0] = 0;
        for (int i = 0, j = 1; j < s.size(); j += 2, i ++) {
            s[j] = _s[i];
            s[j+1] = SPECIAL;
            dp[j] = 0;
            dp[j + 1] = 0;
        }

        int center = 0;
        while (center < s.size()) {
            // First execute a regular radial expansion
            int r, p, m;
            for (r = max(1, dp[center] + 1), p = center + r, m = center - r; m >= 0 && p < s.size(); r ++, m --, p ++) {
                if (s[m] != s[p]) break;
            }
            r --;
            dp[center] = r;

            #ifdef DEBUG
            print_dp(dp);
            #endif

            #ifdef DEBUG
            assert(center - r <= center + r);
            assert(0 <= center - r && center + r < s.size());
            assert(s[center - r] == s[center - r]);
            #endif

            // Next, we'll want to go back through the radial expansion (it's OK to do this second pass
            // because it won't change asymptotics and make our life easy w.r.t.
            // finding palindromes IN the palindrome)
            if (r > 0) {
                int R = r;
                int p, m;
                bool reset = true;
                for (int r = 1, p = center + 1, m = center - 1; m >= center - R && p <= center + R; r ++, m --, p ++) {
                    if (dp[m] > 0) {
                        dp[p] = min(dp[m], center + R - p);
                        if (center - R >= m - dp[m]) {
                            reset = false;
                            center = p;
                            break;
                        }
                    }
                }
                if (reset) center += R;
            } else center ++;
        }

        int max_center = max_element(dp.begin(), dp.end()) - dp.begin();
        int max_r = dp[max_center];

        #ifdef DEBUG
        assert(0 <= max_center - max_r && max_center + max_r < s.size());
        #endif

        vector<char> ans;
        for (int j = max_center - max_r; j < max_center + max_r; j ++) {
            if (s[j] != SPECIAL) ans.push_back(s[j]);
        }
        return string(ans.begin(), ans.end());
    }
};