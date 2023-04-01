/// This is redundant and I realizeda bit late... took more iteration than I wanted >:(
class FreqStack {
private:
    int highest_freq;
    unordered_map<int, map<uint, int>> freq2orders;
    unordered_map<int, vector<uint>> val2order_history;
    unordered_map<int, int> val2freq;
    uint next_order;
    // Given an already updated set of orders and knowledge of the previous orders
    // move from one bucket to another. If either order is not defined
    // (or either frequency is non-existent) it is ignored.
    void move(int val, int f_new, int f_old, uint new_order, uint old_order) {
        // Remove old if necessary
        if (f_old > 0) {
            auto& orders = freq2orders[f_old];
            orders.erase(old_order);
        }
        // Add new if possible
        if (f_new > 0) {
            // Initialize new if necessary
            if (freq2orders.find(f_new) == freq2orders.end()) {
                freq2orders[f_new] = map<uint, int>();
            }
            // Insert into the new frequency
            auto& orders = freq2orders[f_new];

            // Pick either a small starting point or
            // a value bigger than the biggest in the order set
            orders[new_order] = val;
        }
    }
    uint get_order(int val) {
        if (val2order_history.find(val) == val2order_history.end()) return 0;
        auto& order_history = val2order_history[val];
        return order_history[order_history.size() - 1];
    }
    // Update the order of a value and then return the new order
    uint update_order(int val, bool up) {
        // All orders will >= 1 so 0 is understandable
        // NOTE: you sohuld NEVER pass up = false if you have not inserted the element before
        if (val2freq.find(val) == val2freq.end()) {
            val2order_history[val] = vector<uint>();
        }
        auto& orders = val2order_history[val];
        if (!up) {
            orders.pop_back();
            if (orders.size() == 0) {
                val2order_history.erase(val);
                return 0;
            }
            return orders[orders.size() - 1];
        }

        int order = ++next_order;
        val2order_history[val].push_back(order);
        return order;
    }
    // Get the frequency of a given value
    int get_freq(int val) {
        if (val2freq.find(val) == val2freq.end()) return 0;
        return val2freq[val];
    }
    // Update the frequency of a value and return the new frequenc
    int update_freq(int val, bool up) {
        int delta = up ? 1 : -1;
        // NOTE: you should NEVER pass up = false if you are inserting a new element
        int f = val2freq.find(val) == val2freq.end() ? 0: val2freq[val];
        int fp = f + delta;
        if (fp == 0) {
            val2freq.erase(val);
        } else {
            val2freq[val] = fp;
        }

        if (fp > highest_freq) highest_freq ++;
        // NOTE: this assumes you will call move later
        // (updates highest frequency if you are about to remove the last element with that freq.)
        if (f > 0 && freq2orders[f].size() <= 1 && f == highest_freq) highest_freq --;
        return fp;
    }
    // Return the most recently added highest ordered element
    int get_poppable() {
        auto& orders = freq2orders[highest_freq];
        return (orders.rbegin())->second;
    }
    // Helper For Debugging
    void print_info() {
        cout << "********************\n";
        // Print out counts
        cout << "There are " << val2freq.size() << " = " << val2order_history.size() << " unique elements\n";
        cout << "The most frequent element(s) appear(s) " << highest_freq << " times\n";
        cout << "The next order is " << next_order << "\n";
        // Print out hashmaps
        cout << "Val2Order:\n";
        for (auto vo : val2order_history) { cout << "\t" << vo.first << ": " << vo.second[vo.second.size() - 1] << "\n"; }
        cout << "Val2Freq:\n";
        for (auto vf : val2freq) { cout << "\t" << vf.first << ": " << vf.second << "\n"; }
        // Print out buckets
        cout << "Buckets:\n";
        for (auto bucket : freq2orders) {
            cout << "\tFreq " << bucket.first << "\n";
            for (auto elem : bucket.second) { cout << "\t\tElem " << elem.second << " has order " << elem.first << "\n"; }
        }
        cout << "********************\n";
    }
public:
    FreqStack() {
        highest_freq = 0;
        next_order = 0;
    }
    
    void push(int val) {
        // Update orders
        uint old_order = get_order(val);
        uint new_order = update_order(val, true);

        // Update frequency for val and highest frequency
        int f_old = get_freq(val);
        int f_new = update_freq(val, true);
        
        // Move to right bucket changing the order if nec.
        move(val, f_new, f_old, new_order, old_order);
        // print_info(); // XXX
    }
    
    int pop() {        
        cout << "------- POP\n";
        // Find the value to pop
        int val = get_poppable();

        // Update orders
        uint old_order = get_order(val);
        uint new_order = update_order(val, false);

        // Update highest frequency if necessary
        int f_old = highest_freq;
        int f_new = update_freq(val, false);

        // Move, keeping the same order
        move(val, f_new, f_old, new_order, old_order);
        // print_info(); // XXX
        return val;
    }
};

/**
 * Your FreqStack object will be instantiated and called as such:
 * FreqStack* obj = new FreqStack();
 * obj->push(val);
 * int param_2 = obj->pop();
 */