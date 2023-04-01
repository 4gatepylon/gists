// O(log(n))
// to make O(1) use a two sided queue
class MinStack {
private:
    vector<int> internal;
    std::map<int, int> count;
public:
    MinStack() {
        
    }
    
    void push(int val) {
        internal.push_back(val);
        if (count.count(val) == 0) {
            count[val] = 0;
        }
        count[val] ++;
    }
    
    void pop() {
        int val = internal[internal.size() - 1];
        internal.pop_back();
        count[val] --;
        if (count[val] == 0) {
            count.erase(val);
        }
    }
    
    int top() {
        return internal[internal.size() - 1];
    }
    
    int getMin() {
        // Min 
        return count.begin()->first;
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */