// Helpful for debugging
#undef DEBUG
// #define DEBUG
// class __ListNode {
// public:
//     int v;
//     int k;
//     __ListNode* n;
//     __ListNode* p;
//     __ListNode(__ListNode* prev, __ListNode* next, int val, int key) {
//         n = next;
//         p = prev;
//         v = val;
//         k = key;
//     }
// };

// All operations O(logn)
class LRUCache {
private:
    // ListNodes store the value and the keys know which
    // pointer to take. We then use the head and take to keep track
    // of recency (i.e. every time we use something it moves to the head)
    // (and if necessary tail is updated, upon eviction the tail is what is
    // removed)
    // __ListNode* head;
    // __ListNode* tail;
    int head;
    int tail;
    // unordered_map<int, __ListNode*> k2l;
    unordered_map<int, int> k2l;
    int cap;
    int end;
    //n, p, k, v
    int nodes[10001][4];
    
    // DEBUG
    // void validate(bool after, bool put) {
    //     cout << "VALIDATING" << (after ? " AFTER " : " BEFORE ") << " CALLING A " << (put ? " PUT" : " GET") << "\n";
    //     assert(cap >= 0);
    //     assert(k2l.size() <= cap);
    //     if (head != nullptr) {
    //         __ListNode* leader = head->n;
    //         __ListNode* follower = head;
    //         while (leader != nullptr) {
    //             assert(leader->p == follower);
    //             assert(follower->n == leader);
                
    //             leader = leader->n;
    //             follower = follower->n;
    //         }
            
    //     }
    //     cout << "VALIDATING OK\n";
    // }
    void evict() {
        // Remove this from the map so we have one less element
        k2l.erase(nodes[tail][2]);

        // The tail is the element before the tail
        // __ListNode* new_tail = tail->p;
        int new_tail = nodes[tail][0];
        if (new_tail != -1) {
            // Make sure that no-one is pointing to this old tail
            nodes[new_tail][1] = -1;

            // Disconnect in case there's garbage collection or smth.
            nodes[tail][0] = -1;
            nodes[tail][1] = -1;

            // Update tail
            tail = new_tail;
        } else {
            // I assume that we won't have test-cases like this but if the tail becomes nullptr
            // you can be that means that the capacity is one
            // DEBUG
            assert(head == tail);
            assert(cap == 1);
            head = -1;
            tail = -2;
        }
    }
    void update_recency(int key, int val) {
        if (k2l.find(key) == k2l.end()) {
            // If an element is not present first create it
            // It should point prev to nothing and next to the current head
            // __ListNode* new_head = new __ListNode(nullptr, head, val, key);
            int new_head = end++;
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
            // __ListNode* entry = k2l[key];
            int entry = k2l[key];

            // If it is the head it is already at the front
            if (entry == head) return;
            else {
                // The node before now points AFTER it
                // THe node after now points BEFORE it
                // __ListNode* prev_entry = entry->p;
                // __ListNode* next_entry = entry->n;
                int prev_entry = nodes[entry][0];
                int next_entry = nodes[entry][1];
                // prev_entry->n = entry->n;
                if (prev_entry != -1) nodes[prev_entry][1] = next_entry;
                // if (next_entry != nullptr) next_entry->p = prev_entry;
                if (next_entry != -1) nodes[next_entry][0] = prev_entry;
                // It points as head would to the next head and before it
                // entry->p = nullptr;
                // entry->n = head;
                nodes[entry][0] = -1;
                nodes[entry][1] = head;

                // Head points to it because it follows it now
                // if (head != nullptr) head->p = entry;
                if (head == -1) nodes[head][0] = entry;

                // Tail must be updated if this was tail
                if (tail == entry) tail = prev_entry;

                // It is head
                head = entry;
            }
        }
    }
    // DEBUG
    // void log(bool put, int key) {
    //     cout << (put ? "put " : "get ") << key << "\n";
    // }
    // DEBUG
    // void print_info() {
    //     // Print info about the state of our data-structures
    //     // for debugging
    //     cout << "-------------------------------\n";
    //     if (head != nullptr) {
    //         __ListNode* r = head;
    //         while (r != nullptr) {
    //             cout << r->k << ": " << r->v << " <-> ";
    //             r = r->n;
    //         }
    //         cout << "\n";
    //         for (auto& kv : k2l) {
    //             cout << kv.first << ": " << (kv.second)->v << ", ";
    //         }
    //         cout << "\n";
    //     } else cout << "head is nullptr\n";
    //     cout << "-------------------------------\n";
    // }
public:
    LRUCache(int capacity) {
        head = -1;
        tail = -1;
        cap = capacity;
        end = 0;
    }
    
    int get(int key) {
        // DEBUG
        // #ifdef DEBUG
        // validate(false, false);
        // log(false, key);
        // #endif

        auto found = k2l.find(key);
        if (found == k2l.end()) return -1;
        else {
            update_recency(key, nodes[key][3]);
            // DEBUG
            // #ifdef DEBUG
            // validate(true, false);
            // print_info();
            // #endif
            return nodes[key][3];
        }
    }
    
    void put(int key, int value) {
        // DEBUG
        // #ifdef DEBUG
        // validate(false, true);
        // log(true, key);
        // #endif
        
        if (k2l.find(key) != k2l.end()) nodes[key][3] = value;
        else if (k2l.size() >= cap)     evict();
        update_recency(key, value);

        // DEBUG
        // #ifdef DEBUG
        // print_info();
        // validate(true, true);
        // #endif
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */