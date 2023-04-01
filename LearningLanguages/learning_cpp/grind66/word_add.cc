class TrieNode {
    // Only designed for lowercase english letters
    // Tells you if a word is contained or not, while also
    // allowing you to insert words
private:
    // State
    TrieNode* children [26];
    bool terminates;
    // Helpers
    bool _contains(string s, int i) {
        if (i >= s.size()) return this->terminates;
        if (s[i] == '.') {
            bool any_contains = false;
            for (int j = 0; j < 26; j++) {
                if (this->children[j] != nullptr && this->children[j]->_contains(s, i+1)) return true;
            }
            return false;
        }
        TrieNode* c = this->children[s[i] - 'a'];
        return c != nullptr && c->_contains(s, i + 1);
    }
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
    TrieNode(bool terminates) {
        this->terminates = terminates;
        for (int i = 0; i < 26; i++) { this->children[i] = nullptr; }
    }
    bool contains(string s) { return _contains(s, 0); }
    void insert(string s) { _insert(s, 0); }
};
class WordDictionary {
private:
    TrieNode* root;
    unordered_set<string> contains;
public:
    WordDictionary() {
        root = new TrieNode(false);
    }
    
    void addWord(string word) {
        root->insert(word);
        contains.insert(word);
    }
    
    bool search(string word) {
        if (contains.find(word) != contains.end()) return true;
        return root->contains(word);
    }
};

/**
 * Your WordDictionary object will be instantiated and called as such:
 * WordDictionary* obj = new WordDictionary();
 * obj->addWord(word);
 * bool param_2 = obj->search(word);
 */