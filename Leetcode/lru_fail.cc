// Helpful for debugging
#undef DEBUG
// #define DEBUG

// All operations O(logn)
class LRUCache {
private:
    // Key to value
    unordered_map<int, int> k2v;
    // Key to recency
    unordered_map<int, int> k2r;
    // Keep keys sorted by recency
    // and maps a recency to a key (note
    // that every key gets a unique recency)
    map<int, int> r2k;
    int size;
    int cap;
    int recency;
    void validate() {
        assert(size <= cap);
        assert(0 <= size);
        assert(0 <= recency);
    }
    void evict() {
        auto lru = r2k.begin();
        assert(lru != r2k.end());

        // Remove that least recently used key
        assert(r2k.find(lru->first) != r2k.end());
        assert(k2r.find(lru->second) != k2r.end());
        int rec = lru->first;
        int key = lru->second;
        k2r.erase(key);
        r2k.erase(rec);

        // Remove the key from k2v
        k2v.erase(key);
    }
    void update_recency(int key) {
        if (k2r.find(key) != k2r.end()) {
            assert(r2k.find(k2r[key]) != r2k.end());
            r2k.erase(k2r[key]);
        }
        k2r[key] = recency ++;
        r2k[k2r[key]] = key;
        assert(k2r.find(key) != k2r.end());
        assert(r2k.find(k2r[key]) != r2k.end());
    }
    void log(bool put, int key) {
        cout << (put ? "put " : "get ") << key << "\n";
    }
    void print_info() {
        cout << "-----------\n";
        for (auto& p : k2v) {
            cout << p.first << ": " << p.second << " [" << k2r[p.first] << "]\n";
        }
        cout << "R2K:\n";
        for (auto& p : r2k) {
            cout << p.first << ": " << p.second << ", ";
        }
        cout << "\n-----------\n";
    }
public:
    LRUCache(int capacity) {
        size = 0;
        cap = capacity;
        recency = 0;
    }
    
    int get(int key) {
        #ifdef DEBUG
        validate();
        log(false, key);
        #endif

        auto found = k2v.find(key);
        if (found == k2v.end()) return -1;
        else {
            update_recency(key);
            return found->second;
        }
        #ifdef DEBUG
        print_info();
        #endif
    }
    
    void put(int key, int value) {
        #ifdef DEBUG
        validate();
        log(true, key);
        #endif
        
        if (k2v.find(key) != k2v.end()) {
            k2v[key] = value;
            update_recency(key);
        } else {
            if (size >= cap) evict();
            else size ++;
            k2v[key] = value;
            update_recency(key);
        }
        #ifdef DEBUG
        print_info();
        #endif
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */