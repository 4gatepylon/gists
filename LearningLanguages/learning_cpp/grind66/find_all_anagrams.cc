class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        if (p.size() > s.size()) return vector<int>(0);
        vector<int> idxs;
        int target[26];
        int have[26];
        int num_reqs = 0;
        int num_reqs_met = 0;
        for (int i = 0; i < 26; i++) {
            target[i] = 0;
            have[i] = 0;
        }
        for (char c : p) {
            if (target[c - 'a'] == 0) num_reqs ++;
            target[c - 'a'] ++;
        }
        for (int i = 0; i < s.size(); i++) {
            int start = i - p.size() + 1;
            have[s[i] - 'a'] ++;
            if (have[s[i] - 'a'] == target[s[i] - 'a']) num_reqs_met ++;
            if (num_reqs == num_reqs_met) idxs.push_back(start);
            if (i >= p.size() - 1) {
                if (have[s[start] - 'a'] == target[s[start] - 'a']) num_reqs_met --;
                have[s[start] - 'a'] --;
            }
        }
        return idxs;
    }
};