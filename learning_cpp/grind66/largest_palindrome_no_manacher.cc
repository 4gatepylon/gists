#include <math.h>
class Solution {
private:
    void print_string(vector<char> s) {
        for (int i = 0; i < s.size(); i ++) {cout << s[i];}; cout << "\n";
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
        for (int i = 0, j = 1; j < s.size(); j += 2, i ++) {
            s[j] = _s[i];
            s[j+1] = SPECIAL;
        }

        int max_i = 0;
        int max_r = 0;
        for (int i = 0; i < s.size(); i ++) {
            int r = 0;
            for (r = 0; i - r >= 0 && i + r < s.size(); r++) {
                if (s[i + r] != s[i - r]) break;
            }
            r --;
            assert(i - r <= i + r); // DEBUG
            assert(0 <= i - r && i + r < s.size()); // DEBUG
            assert(s[i - r] == s[i - r]); // DEBUG
            dp[i] = r;
            if (r > max_r) {
                max_i = i;
                max_r = r;
            }
        }

        assert(0 <= max_i - max_r && max_i + max_r < s.size()); // DEBUG
        vector<char> ans;
        for (int j = max_i - max_r; j < max_i + max_r; j++) {
            if (s[j] != SPECIAL) ans.push_back(s[j]);
        }
        return string(ans.begin(), ans.end());
    }
};