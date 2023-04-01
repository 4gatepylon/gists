class Solution {
pair<int, int> eat_num(string& s, int i) {
    int red = 0;
    while (i < s.size() && '0' <= s[i] && s[i] <= '9') {
        red *= 10;
        red += s[i] - '0';
        i++;
    }
    return pair<int, int>(red, i);
}
pair<int, int> eat_paren(string& s, int i) {
    pair<int, int> inner = eat(s, i + 1);
    return pair<int, int>(inner.first, inner.second + 1);
}
pair<int, int> eat(string& s, int i) {
    int prev_op = 1;
    int acc = 0;
    int j = i;
    while (j < s.size() && s[j] != ')') {
        if (s[j] == '-') {
            prev_op *= -1;
            j++;
        } else if (s[j] == '+') {
            j++;
        } else {
            // This is used for debugging impossible situations
            #define INVALID 999999

            // Eat the next value
            pair<int, int> eaten;
            if (s[j] == '(') eaten = eat_paren(s, j);
            else if ('0' <= s[j] && s[j] <= '9') eaten = eat_num(s, j);
            else eaten = pair<int, int>(INVALID, INVALID);

            // Accumulate the value
            acc += prev_op * eaten.first;
            prev_op = 1;
            j = eaten.second;
        }
    }
    return pair<int, int>(acc, j);
}
public:
    int calculate(string s) {
        vector<char> vec;
        for (char c : s) {
            if (c != ' ') {
                vec.push_back(c);
            }
        }
        string str(vec.begin(), vec.end());
        return eat(str, 0).first;
    }
};