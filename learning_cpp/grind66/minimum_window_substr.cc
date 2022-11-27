#include <string.h>
class Solution {
public:
    // Greedy. Look at where you end. O(n)
    string minWindow(string s, string t) {
        // We will check that we are meeting each of the conditions
        // Each time you add a new letter or remove a new letter you'll
        //   check if you are breaking or meeting a condition by comparing
        //   the entry in the two dictionaries.
        int num_conditions = 0;
        int num_conditions_met = 0;
        unordered_map<char, int> has;
        unordered_map<char, int> must_have;
        for (char c : t) {
            if (must_have.find(c) == must_have.end()) {
                num_conditions ++;
                must_have[c] = 1;
                has[c] = 0;
            } else {
                must_have[c] ++;
            }
        }
        // Fill up with empty entries for c because it makes it easier to write the code later
        for (char c : s) {
            if (has.find(c) == has.end()) has[c] = 0;
            if (must_have.find(c) == must_have.end()) must_have[c] = 0;
        }

        int i_opt   = -1;
        int len_opt = INT_MAX;
        int i = 0;
        int j = 0;
        // First, find the first valid window that meets the conditions
        while (j < s.size() && num_conditions_met < num_conditions) {
            has[s[j]] ++;
            if (must_have[s[j]] == has[s[j]]) num_conditions_met ++;
            j++;
        }
        if (num_conditions_met < num_conditions) return "";
        while(j <= s.size()) {
            // Now, if we have met the conditions, we simply need to shrink the left side and then increment the
            // right side greedily.
            while (i < s.size() && must_have.find(s[i]) == must_have.end() || must_have[s[i]] < has[s[i]]) {
                has[s[i]] --;
                i ++;
            }

            // See if the substring up until this j (exclusive) works
            if (j - i < len_opt) {
                cout << i << " " << j << "\n";
                i_opt = i;
                len_opt = j - i;
            }

            // Go to the next j
            if (j < s.size()) has[s[j]] ++;
            j ++;
        }
        if (i_opt == -1) return "";
        return std::string(s.c_str() + i_opt, s.c_str() + i_opt + len_opt);
    }
};