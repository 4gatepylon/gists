class TrieNode {
    // Only designed for lowercase english letters
    // Tells you if a word is contained or not, while also
    // allowing you to insert words
private:
    void _insert(string s, int i) {
        if (i >= s.size()) {
            this->terminates = true;
            return;
        }
        TrieNode* c = this->children[s[i] - 'a'];
        if (c == nullptr) {
            c = new TrieNode(false);
            this->children[s[i] - 'a'] = c;
        }
        c->_insert(s, i+1);
    }
public:
    TrieNode* children [26];
    bool terminates;
    TrieNode(bool terminates) {
        this->terminates = terminates;
        for (int i = 0; i < 26; i++) { this->children[i] = nullptr; }
    }
    void insert(string s) { _insert(s, 0); }
};
class Solution {
private:
    vector<int> solvable_from_root;
    bool solve(string s, TrieNode* root, TrieNode* wd, int i) {
        if (i >= s.size()) return wd->terminates;
        if (root == wd && solvable_from_root[i] != -1) return solvable_from_root[i] == 1;
        
        bool ans = false;
        int c = s[i] - 'a';
        if(wd->children[c] != nullptr || wd->terminates) {
            // imporant to greedily try to solve with root first because
            // we can maximize our use of the cache that way
            ans = ans || wd->terminates && solve(s, root, root, i);
            ans = ans || wd->children[c] != nullptr && solve(s, root, wd->children[c], i + 1);
        }
        if (wd == root) solvable_from_root[i] = 0;
        return ans;
    }
public:
    // Could be better with a running hash function
    bool wordBreak(string s, vector<string>& wordDict) {
        solvable_from_root.clear();
        // Will be 0 for not solvable,
        // Will be 1 for solvable
        // Will be -1 for not yet solved
        for (int i = 0; i < s.size(); i++) { solvable_from_root.push_back(-1); }
        TrieNode* root = new TrieNode(false);
        for (string word : wordDict) {
            root->insert(word);
        }
        return solve(s, root, root, 0);
    }
};