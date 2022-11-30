// hate stl
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

class Solution {
private:
    // Store at [target] which indices to use
    vector<vector<int>> memo;
    vector<bool>        visited;
    // For DFS after DP first pass
    unordered_set<vector<int>> sols;
    vector<int>                sol; 
    void cs(unordered_set<int>& candidates, int target) {
        assert(target >= 0);              // DEBUG
        assert(memo[target].size() >= 0); // DEBUG
        if (target <= 0 || visited[target]) return;

        for (int c : candidates) {
            if (target - c > 0) {
                cs(candidates, target - c);
                if (memo[target - c].size() > 0) memo[target].push_back(c);
            } else if (target - c == 0) {
                memo[target].push_back(c);
            }
        }
        visited[target] = true;
    }
    void print_sol() {
        for (int i = 0; i < sol.size(); i++) {
            cout << " " << sol[i];
        }
        cout << "\n";
    }
    void print_memo() {
        for (int i = 0; i < memo.size(); i++) {
            cout << i << ":\n";
            for (int c : memo[i]) {
                cout << "\t" << c << "\n";
            }
        }
    }
    void dfs(int target) {
        // cout << "DFS " << target << "\n"; // DEBUG
        // Insert will avoid duplication
        if (target == 0) {
            // print_sol(); // DEBUG
            vector<int> soll(sol.begin(), sol.end());
            sort(soll.begin(), soll.end());
            sols.insert(soll);
        }
        else {
            for (int c : memo[target]) {
                // cout << "DFSing from " << target << " TO " << target - c << " VIA " << c << "\n"; // DEBUG
                sol.push_back(c);
                dfs(target - c);
                sol.pop_back();
                // cout << "DONE WITH `from " << target << " TO " << target - c << " VIA " << c << "`\n"; // DEBUG
            }
        }
    }
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        memo.clear();
        visited.clear();
        for (int i = 0; i < target + 1; i++) { visited.push_back(false); }
        for (int i = 0; i < target + 1; i++) { memo.push_back(vector<int>()); }

        // Use DP to find the solutions in the form of a tree
        unordered_set<int> unique_cands(candidates.begin(), candidates.end());
        cs(unique_cands, target);

        // print_memo(); // DEBUG

        // Use DFS to create lists from those tree elements (we will only have to copy ONCE)
        sols.clear();
        sol.clear();
        dfs(target);   

        vector<vector<int>> returnable;
        for (const vector<int>& s : sols) {
            returnable.push_back(vector<int>(s.begin(), s.end()));
        }
        return returnable;
    }
};