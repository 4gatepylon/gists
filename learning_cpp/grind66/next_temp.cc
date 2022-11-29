class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        vector<int> ans(temperatures.size(), 0);
        vector<int> stack;
        for (int i = temperatures.size() - 1; i >= 0; i--) {
            while (stack.size() > 0 && temperatures[stack[stack.size() - 1]] <= temperatures[i]) stack.pop_back();
            if (stack.size() > 0) ans[i] = stack[stack.size() - 1] - i;
            stack.push_back(i);
        }
        return ans;
    }
};