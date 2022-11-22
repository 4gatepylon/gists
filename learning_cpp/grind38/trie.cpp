#ifndef NULL
#define NULL 0
#endif

typedef struct {
    bool finishes;
    int mapId;
} TrieNode;

class Trie {
public:
    Trie() {
        _root = (TrieNode*)malloc(sizeof(TrieNode));
        _root->finishes = false;
        _root->mapId = 0;
        _allNodesOnHeap.push_back(_root);
        _nextMapId = 1;
    }
    ~Trie() {
        for (TrieNode* t : _allNodesOnHeap) {
            free(t);
        }
    }
    
    void insert(string word) {
        TrieNode* curr = _root;
        for (char c : word) {
            // Create the map if necessary
            int mapId = curr->mapId;
            if (_children.count(mapId) == 0) {
                _children[mapId] = unordered_map<char, TrieNode*>();
            }
            // Create the child if necessary
            if (_children[mapId].count(c) == 0) {
                TrieNode* newChild = (TrieNode*)malloc(sizeof(TrieNode));
                newChild->finishes = false;
                newChild->mapId = _nextMapId;
                _nextMapId ++;
                _allNodesOnHeap.push_back(newChild);

                _children[mapId][c] = newChild;
            }
            curr = _children[mapId][c];
        }
        curr->finishes = true;
    }
    
    // Return NULL if not reachable
    TrieNode* reach(string word) {
        TrieNode* curr = _root;
        for (char c : word) {
            int mapId = curr->mapId;
            if (_children.count(mapId) == 0 || _children[mapId].count(c) == 0) {
                return NULL;
            }
            curr = _children[mapId][c];
        }
        return curr;
    }

    bool search(string word) {
        TrieNode* end = reach(word);
        return end != NULL && end->finishes == true;
    }
    
    bool startsWith(string prefix) {
        return reach(prefix) != NULL;
    }
private:
    // First TrieNode
    TrieNode* _root;

    // Store a map from mapIds (so a map of maps) to a map for each trie node
    // to its children. Do it this way so when this object is destructed all this
    // can go with it;
    unordered_map<int, unordered_map<char, TrieNode*>> _children;
    
    // Keep track of the map ids so they are unique
    int _nextMapId;

    // Because all TrieNodes are on the heap, we keep this here so we could
    // theoretically deallocate them in the destructor
    vector<TrieNode*> _allNodesOnHeap;
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */