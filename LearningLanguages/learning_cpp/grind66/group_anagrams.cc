class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> sols;
        if(strs.size() == 0) return sols;

        unordered_map<string, vector<string>> groups;
        for (string _s : strs) {
            // Copy 1
            string s = string(_s.begin(), _s.end());
            sort(s.begin(), s.end());
            if (groups.find(s) == groups.end()) {
                // Copy 2
                // These vectors returned out of the function
                groups[s] = vector<string>(1, string(_s.begin(), _s.end()));
                assert(groups.size() > 0);
            } else {
                // Le copy
                groups[s].push_back(string(_s.begin(), _s.end()));
                assert(groups.size() > 0);
            }
        }
        for (auto& kv : groups) {
            sols.push_back(kv.second);
        } 
        return sols;
    }
};