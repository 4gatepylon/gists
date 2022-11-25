// https://quantum5.ca/2019/04/05/using-unordered-data-structures-on-std-pair-cpp/
namespace std {
    template<>
    struct hash<std::vector<int>> {
        std::size_t operator()(const std::vector<int> &nums) const {
            // It's ok to put it in zero because we won't be making multiple copies of this
            if (nums.size() == 0) return 0;
            std::size_t h = 0;
            for (int n : nums) {
                h = h ^ std::hash<int>()(n);
            }
            return h;
        }
    };
}

vector<vector<int>> unique(vector<vector<int>> v) {
    std::unordered_set<vector<int>> s(v.begin(), v.end());
    std::vector<vector<int>> out;
    out.insert(out.end(), s.begin(), s.end());
    return out;
}

class Solution {
private:
    unordered_map<int, vector<vector<int>>> memo;
    vector<vector<int>> cs(vector<int>& candidates, int target) {
        // No way to make negatives for positive candidates and targets
        if (target < 0) return vector<vector<int>>(0);

        // One way to make zero (i.e. with nothing)
        if (target == 0) return vector<vector<int>>(1, vector<int>(0));

        // If we have a solution already return it
        if (memo.count(target) > 0) return memo[target];

        // Otherwise create solution from solutions of smaller problems
        vector<vector<int>> sol;
        for (int c : candidates) {
            // Get the solution the the smaller problem
            vector<vector<int>> csol_memo = combinationSum(candidates, target - c);

            // Deep copy the vector and modify it (adding the new element)
            vector<vector<int>> csol;
            for (vector<int> csol_memo_inst : csol_memo) {
                vector<int> csol_inst;
                csol_inst.insert(csol_inst.end(), csol_memo_inst.begin(), csol_memo_inst.end());
                csol_inst.push_back(c);

                // Sort that our solutions are unique and we don't write them multiple times
                sort(csol_inst.begin(), csol_inst.end());

                csol.push_back(csol_inst);
            }

            // Add the solutions for this c to all the solutions
            sol.insert(sol.end(), csol.begin(), csol.end());
        }

        // Make sure we don't duplicate solutions
        // (can happen if you can decrease by say x or y and you choose x then y and y then x)
        // (there is probably a more clever way to avoid this...)
        sol = unique(sol);

        // Update the memo so that we don't have to do this work again and return
        memo[target] = sol;
        return sol;
    }
public:
    // Expect this to take roughly, for a target of size T and minimum candidate of size c
    // O(N(T/c)) where N is the number of solutions that can exist to a target. Probably N ~ exponential
    // so this is not ideal... at least we have no exponential problems w.r.t. recursion depth
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        memo.clear();
        return cs(candidates, target);
    }
};