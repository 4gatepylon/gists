class Solution {
private:
    // 0 and 1 not used
    char opts[10][4] = {{'\0','\0','\0','\0'},{'\0','\0','\0','\0'},
        {'a','b','c','\0'},
        {'d','e','f','\0'},
        {'g','h','i','\0'},
        {'j','k','l','\0'},
        {'m','n','o','\0'},
        {'p','q','r','s'},
        {'t','u','v','\0'},
        {'w','x','y','z'}};
    vector<string> combos;
    vector<char> combo;
    void dfs(string digits, int i) {
        if (i == digits.size()) {
            combos.push_back(string(combo.begin(), combo.end()));
        } else {
            for (char c : opts[digits[i] - '0']) {
                if (c != '\0') {
                    combo.push_back(c);
                    dfs(digits, i + 1);
                    combo.pop_back();
                }
            }
        }
    }
public:
    vector<string> letterCombinations(string digits) {
        if (digits.size() == 0) return vector<string>();
        combos.clear();
        combo.clear();
        dfs(digits, 0);
        return combos;
    }
};