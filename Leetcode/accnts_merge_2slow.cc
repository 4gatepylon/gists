class Solution {
private:
    unordered_set<int> to_visit;
    vector<int> group;
    int peek(unordered_set<int>& s) {for (int k : s) { return k; } return -1; }
    void dfs(vector<vector<int>>& graph, int n) {
        if (to_visit.find(n) == to_visit.end()) return;
        to_visit.erase(n);
        group.push_back(n);
        for (int neighbor : graph[n]) {
            if (to_visit.find(neighbor) != to_visit.end()) dfs(graph, neighbor);
        }
    }
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        // Map index i -> j if accounts[i] shares an email with accounts[j]
        // (create a graph) because connected components will be the ones that are merged
        vector<vector<int>> g(accounts.size(), vector<int>(0));
        for (int i = 0; i < accounts.size(); i++) {
            for (int j = i + 1; j < accounts.size(); j++) {
                unordered_set<string> mails(accounts[i].begin() + 1, accounts[i].end());
                for (string s : accounts[j]) {
                    if (mails.find(s) != mails.end()) {
                        g[i].push_back(j);
                        g[j].push_back(i);
                        break;
                    }
                }
            }
        }
        
        // Perform DFS until we have no more elements left to group
        to_visit.clear();
        for (int i = 0; i < accounts.size(); i++) { to_visit.insert(i); }

        vector<vector<int>> groups;
        while (to_visit.size() > 0) {
            group.clear();
            dfs(g, peek(to_visit));
            groups.push_back(vector<int>(group.begin(), group.end()));
        }

        // Merge the emails
        vector<vector<string>> email_groups;
        for (vector<int> group : groups) {
            unordered_set<string> mails;
            string name = "";
            for (int i : group) {
                if (name == "") name = accounts[i][0];
                for (int j = 1; j < accounts[i].size(); j++) {
                    mails.insert(accounts[i][j]);
                }
            }
            vector<string> email_group(mails.begin(), mails.end());
            email_group.push_back(email_group[0]);
            email_group[0] = name;
            sort(email_group.begin() + 1, email_group.end());

            email_groups.push_back(email_group);
        }

        return email_groups;
    }
};