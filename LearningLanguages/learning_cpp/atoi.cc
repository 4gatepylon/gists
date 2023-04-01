#include <bits/stdc++.h>
class Solution {
public:
    int myAtoi(string s) {
        int start = 0;
        int mult = 1;
        long count = 0;
        while (s[start] == ' ') start++;
        if (s[start] == '-') mult = -1;
        if (s[start] == '+' || s[start] == '-') start++;
        while (start < s.size() && '0' <= s[start] && s[start] <= '9') {
            count *= 10;
            count += s[start] - '0';
            if (mult == 1 && count >= INT_MAX) return INT_MAX;
            if (mult == -1 && -count <= INT_MIN) return INT_MIN;
            start++;
        }
        return mult*count;
    }
};