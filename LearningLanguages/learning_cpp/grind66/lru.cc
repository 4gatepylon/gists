class LRUCache {
private:
    int head;
    int tail;
    unordered_map<int, int> k2l;
    int cap;
    //n, p, k, v
    int nodes[3000][4];
    vector<int> nulls;
    
    // DEBUG
    void validate(bool after, bool put) {
        cout << "VALIDATING" << (after ? " AFTER " : " BEFORE ") << " CALLING A " << (put ? " PUT" : " GET") << "\n";
        assert(cap >= 0);
        assert(k2l.size() <= cap);
        if (head != -1) {
            int leader = nodes[head][1];
            int follower = head;
            while (leader != -1) {
                assert(nodes[leader][0] == follower);
                assert(nodes[follower][1] == leader);
                
                leader = nodes[leader][1];
                follower = nodes[follower][1];
            }
            
        }
        cout << "VALIDATING OK\n";
    }
    void log(bool put, int key) {
        cout << (put ? "put " : "get ") << key << "\n";
    }
    void print_info() {
        // Print info about the state of our data-structures
        // for debugging
        cout << "-------------------------------\n";
        if (head != -1) {
            int r = head;
            while (r != -1) {
                cout << nodes[r][2] << ": " << nodes[r][3] << " <-> ";
                r = nodes[r][1];
            }
            cout << "\n";
            for (auto& kv : k2l) {
                cout << kv.first << ": " << nodes[kv.second][3] << ", ";
            }
            cout << "\n";
        } else cout << "head is nullptr\n";
        cout << "-------------------------------\n";
    }
    // HELPERS
    void evict() {
        // Remove this from the map so we have one less element
        k2l.erase(nodes[tail][2]);

        // The tail is the element before the tail
        int new_tail = nodes[tail][0];
        if (new_tail != -1) {
            // Make sure that no-one is pointing to this old tail
            nodes[new_tail][1] = -1;

            // Disconnect in case there's garbage collection or smth.
            nodes[tail][0] = -1;
            nodes[tail][1] = -1;
            nulls.push_back(tail);

            // Update tail
            tail = new_tail;
        } else {
            // I assume that we won't have test-cases like this but if the tail becomes nullptr
            // you can be that means that the capacity is one
            // DEBUG
            assert(head == tail);
            assert(cap == 1);
            head = -1;
            tail = -1;
            // There may or may not be something here to do with nulls
        }
    }
    void update_recency(int key, int val) {
        if (k2l.find(key) == k2l.end()) {
            // If an element is not present first create it
            // It should point prev to nothing and next to the current head
            int new_head = nulls[nulls.size() - 1];
            nulls.pop_back();
            nodes[new_head][0] = -1;
            nodes[new_head][1] = head;
            nodes[new_head][2] = key;
            nodes[new_head][3] = val;

            // If there is no current head then we need ot make sure this is also the tail
            if (k2l.size() == 0) tail = new_head;
            else {
                // If there is a current head it needs to point back here instead of nullptr
                nodes[head][0] = new_head;
            }
            // Update head
            // Update the map pointing to this linked-list node
            head = new_head;
            k2l[key] = new_head;
        } else {
            // Find the listnode in the linked-list
            int entry = k2l[key];

            // If it is the head it is already at the front
            if (entry == head) return;
            else {
                // The node before now points AFTER it
                // The node after now points BEFORE it
                int prev_entry = nodes[entry][0];
                int next_entry = nodes[entry][1];
                if (prev_entry != -1) nodes[prev_entry][1] = next_entry;
                if (next_entry != -1) nodes[next_entry][0] = prev_entry;
                // It points as head would to the next head and before it
                nodes[entry][0] = -1;
                nodes[entry][1] = head;

                // Head points to it because it follows it now
                if (head != -1) nodes[head][0] = entry;

                // Tail must be updated if this was tail
                if (tail == entry) tail = prev_entry;

                // It is head
                head = entry;
            }
        }
    }
public:
    LRUCache(int capacity) {
        head = -1;
        tail = -1;
        cap = capacity;
        nulls = vector<int>(3000);
        for (int i = 0; i < 3000; i++) nulls[i] = i;
    }
    
    int get(int key) {
        // DEBUG
        // validate(false, false);
        // log(false, key);

        auto found = k2l.find(key);
        if (found == k2l.end()) return -1;
        else {
            update_recency(key, nodes[k2l[key]][3]);

            // DEBUG
            // validate(true, false);
            // print_info();

            return nodes[k2l[key]][3];
        }
    }
    
    void put(int key, int value) {
        // DEBUG
        // validate(false, true);
        // log(true, key);

        if (k2l.find(key) != k2l.end()) nodes[k2l[key]][3] = value;
        else if (k2l.size() >= cap)     evict();
        update_recency(key, value);

        // DEBUG
        // print_info();
        // validate(true, true);
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */