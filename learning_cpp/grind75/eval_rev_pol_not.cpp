class Solution {
public:
    // typedef the lambdas
    using BinaryOperator = long (*) (long, long);

    long evalRPN(vector<string>& tokens) {
        unordered_map<string, BinaryOperator> functionMap;
        functionMap["+"] = [](long a, long b) {return a + b;};
        functionMap["-"] = [](long a, long b) {return a - b;};
        functionMap["*"] = [](long a, long b) {return a * b;};
        functionMap["/"] = [](long a, long b) {return a / b;};

        pair<long, long> valAndIdx = eval(tokens, tokens.size() - 1, functionMap);
        // Throw an exception basically if we did not consume the entire string
        if (valAndIdx.second != -1) {
            return 1/0;
        }
        return valAndIdx.first;
    }
    // evaluate at index i and return the index to the left that we evaluated until
    // (i.e. the next index you'll want, which would be -1 if we consumed the string)
    pair<long, long> eval(vector<string>& t, long i, unordered_map<string, BinaryOperator> functionMap) {
        // This is our "base case"
        cout << "eval(" << i << ") to left";
        if (functionMap.count(t[i]) == 0) {
            pair<long, long> p;
            p.first = stoi(t[i]);
            p.second = i - 1;
            return p;
        } else {
            pair<long, long> rightValAndIdx = eval(t, i - 1, functionMap);
            pair<long, long> leftValAndIdx = eval(t, rightValAndIdx.second, functionMap);
            pair<long, long> p;
            p.first = functionMap[t[i]](leftValAndIdx.first, rightValAndIdx.first);
            p.second = leftValAndIdx.second;
            return p;
        }
    }
};