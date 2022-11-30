class Solution {
private:
    unordered_map<string, unordered_set<string>> adj;
    vector<vector<string>> ans;
    unordered_set<string> visited;
    void dfs(const string& email) {
        if (visited.find(email) != visited.end()) return;
        else {
            visited.insert(email);
            ans[ans.size() - 1].push_back(email);
            for (const string& n : adj[email]) {
                dfs(n);
            }
        }
    }
    // Helpful for debugging
    void print_adj() {
        for (const auto& kv : adj) {
            cout << kv.first << ":\n";
            for (const auto& n : kv.second) {
                cout << "\t" << n << "\n";
            }
        }
    }
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        unordered_map<string, string> name;
        adj.clear();
        for (int i = 0; i < accounts.size(); i ++) {
            // Create a star
            string& star_email = accounts[i][1];
            auto star_found = adj.find(star_email);
            if (star_found == adj.end()) {
                adj[star_email] = unordered_set<string>();
                name[star_email] = accounts[i][0];
            }
            for (int j = 2; j < accounts[i].size(); j++) {
                string& email = accounts[i][j];
                auto found = adj.find(email);
                if (found == adj.end()) {
                    adj[email] = unordered_set<string>();
                    name[email] = accounts[i][0];
                }
                adj[email].insert(star_email);
                adj[star_email].insert(email);
            }
        }
        // print_adj();

        ans.clear();
        for (auto& emailkv : adj) {
            if (visited.find(emailkv.first) == visited.end()) {
                // Push back the name
                ans.push_back(vector<string>(1, name[emailkv.first]));
                dfs(emailkv.first);
                sort(ans[ans.size() - 1].begin() + 1, ans[ans.size() - 1].end());
            }
        }
        return ans;
    }
};