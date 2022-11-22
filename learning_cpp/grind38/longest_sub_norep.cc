#include <unordered_map>
#include <math.h>

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> prevIdx;
        int startIdx = 0;
        int maxlen = 0;
        for (int i = 0; i < s.length(); i++) {
            if (prevIdx.count(s[i]) > 0 && prevIdx[s[i]] >= startIdx) {
                maxlen = max(maxlen, i - startIdx);
                startIdx = prevIdx[s[i]] + 1;
            }
            prevIdx[s[i]] = i;
        }
        maxlen = max(maxlen, (int)s.length() - startIdx);
        return maxlen;
    }
};