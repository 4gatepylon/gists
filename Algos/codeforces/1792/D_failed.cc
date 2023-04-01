// NOTE: I wrote this a-posteriori

#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>

using namespace std;

// Compile with
/*
g++ D.cc && cat D.txt | ./a.out
*/

// solved the wrong problem

// Does not support deletes nor does it support words of different lengths
// (all leaf nodes are assumed to be terminations of a word)
const size_t MaxAlphabetSize = 10;
class TrieNode {
private:
    int val;
    TrieNode* next[MaxAlphabetSize + 1];
public:
    TrieNode(int val) {
        this->val = val;
        for (int i = 0; i < MaxAlphabetSize + 1; i++) { this->next[i] = nullptr; }
    }
    ~TrieNode() { TrieNode::DeleteChildren(this->next); }
    TrieNode* getNext(int val) { return this->next[val]; }
    void setNext(int val) { this->next[val] = new TrieNode(val); }

    // This is used both by TrieNode and Trie
    static void DeleteChildren(TrieNode* next[]) {
        for (int i = 0; i < MaxAlphabetSize + 1; i++) {
            if (next[i] != nullptr) {
                // https://www.geeksforgeeks.org/delete-in-c/
                // https://stackoverflow.com/questions/4036394/how-do-i-call-the-classs-destructor
                delete next[i];
                next[i] = nullptr;
            }
        }
    }
};

class Trie {
private:
    TrieNode* root[MaxAlphabetSize + 1];
public:
    Trie() { for (int i = 0; i < MaxAlphabetSize + 1; i++) { this->root[i] = nullptr; }}
    ~Trie() { TrieNode::DeleteChildren(this->root); }

    // Insert a word into our Trie
    void insert(vector<int>& word) {
        TrieNode* t = this->root[word[0]];
        // cout << "Inserting " << word[0] << endl; // XXX
        if (t == nullptr) {
            t = new TrieNode(word[0]);
            // cout << "Created a new node but not yet set into the array" << endl; // XXX
            this->root[word[0]] = t;
        }

        // cout << "Created the first node I guess\n"; // XXX
        for (int i = 1; i < word.size(); i++) {
            assert(t != nullptr);
            if (t->getNext(word[i]) == nullptr) { t->setNext(word[i]); }
            t = t->getNext(word[i]);
        }
    }

    // Get the longest existing prefix (of `word`) length of any inserted element into 
    // the Trie such that it was inserted by an element including avoid
    int longestExistingPrefixLength(vector<int>& word) {
        // The prefix is empty
        if (word.size() == 0) return 0;
        // The prefix was never inserted
        if (root[word[0]] == nullptr) return 0;

        int len = 1;
        TrieNode* t = this->root[word[0]];
        for (int i = 1; i < word.size(); i++) {
            // If there is nothing that continues down this prefix we are done
            if (t->getNext(word[i]) == nullptr) return len;

            // Otherwise continue down
            t = t->getNext(word[i]);
            len ++;
        }
        return len;
    }
};

// Helpers basically
namespace D {
    // Permutation that this would index into the identity
    vector<int> createIndexxed(vector<int>& perm) {
        assert(perm.size() <= MaxAlphabetSize);

        vector<int> idxxed(perm.size(), -1);
        for (int i = 0; i < perm.size(); i++) {
            assert(perm[i] >= 1);
            
            // To be indexxed into the identity by `perm` then for element x at index i in `perm`
            // we need to have index x have element i
            // (I think pq = qp if the two are inverses...)
            idxxed[perm[i] - 1] = i + 1;
        }


        for (int i = 0; i < idxxed.size(); i++) assert(idxxed[i] != -1);
        assert(idxxed.size() == perm.size());
        return idxxed;
    }

    // 1 3 2
    // 2 1 3
    // -> 3 1 2

    // 2 1 3
    // 1 3 2
    // -> 2 3 1

    // A(B()) != B(A()) ...
    // Our strategy

    Trie createTrieSet(vector<vector<int> >& perms) {
        Trie trie;
        for (int i = 0; i < perms.size(); i++) {
            // cout << "Inserting " << i << "th permutation into trie" << endl; // XXX
            trie.insert(perms[i]);
        }
        return trie;
    }
};


// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    // Multiplying by a prefix that is correct can only yield that prefix
    // (i.e. two permutations with prefixes of length k1, k2, if you multiply them,
    // you only get a prefix at most of length min(k1, k2))
    // -> best out of all selections s.t. one of the two has a prefix is the 2nd longest prefix
    // can we reverse shit?
    // product of two identical things is identity!

    // For each test find the size of the matrix, then find the most beautiful matrix of that
    // size of a permutation of the numbers 1 to n^2
    for (int i = 0; i < num_tests; i++) {        
        // Get the input parameters so we can read stdin
        int n = -1, m = -1;
        cin >> n;
        cin >> m;
        assert(n >= -1);
        assert(m >= -1);
        assert(MaxAlphabetSize + 1 > m);

        // Get the permutations from stdin and make sure they are sane
        vector<vector<int> > perms(n, vector<int>(m, -1));
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < m; k++) {
                cin >> perms[j][k];
            }
        }
        assert(perms.size() == n);
        for (int j = 0; j < n; j++) {
            assert(perms[j].size() == m);
            for (int k = 0; k < m; k++) {
                assert(perms[j][k] >= 1);
                assert(perms[j][k] <= m);
            }
        }
        // cout << "Parsed perms\n";   // XXX

        // Create a set of the permutations that exist
        Trie set = D::createTrieSet(perms);

        // cout << "Created trie set\n"; // XXX

        // For each permutation, see what the indexxer is and see what the longest
        // present prefix of it is: this is the max beauty of a permutation product that
        // includes this as the second multiplier
        for (int j = 0; j < n; j++) {
            vector<int> idxxed = D::createIndexxed(perms[j]);
            // cout << "************\n"; // XXX
            for (int k = 0; k < perms[j].size(); k++) { cout << perms[j][k] << " "; } cout << endl; // XXX
            for (int k = 0; k < idxxed.size(); k++) { cout << idxxed[k] << " "; } cout << endl; // XXX
            cout << "\n"; // XXX

            // Avoid the current permutation since you need two unique permutations for this
            int maxBeautyThis = set.longestExistingPrefixLength(idxxed);
            cout << "beauty was " << maxBeautyThis << endl; // XXX
            cout << "************\n"; // XXX
            assert(0 <= maxBeautyThis && maxBeautyThis <= m);
            // cout << maxBeautyThis << (j < n - 1 ? " " : ""); // THIS IS WHAT IT SHOULD BE
        }
        cout << "\n";
    }
    return 0;
}