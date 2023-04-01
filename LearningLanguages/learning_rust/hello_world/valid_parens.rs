impl Solution {
    pub fn is_valid(s: String) -> bool {
        let mut stk: Vec<char> = Vec::with_capacity(s.len());
        for e in s.chars() {
            let mut valid = false;
            if e == '(' || e == '[' || e == '{' {
                stk.push(e);
                valid = true;
            } else if stk.len() > 0 {
                let p = stk[stk.len() - 1];
                if e == ')' && p == '(' || e == ']' && p == '[' || e == '}' && p == '{' {
                    stk.pop();
                    valid = true;
                }
            }
            if !valid && (e == ')' || e == ']' || e == '}') {
                return false;
            }
        }
        return stk.is_empty();
    }
}