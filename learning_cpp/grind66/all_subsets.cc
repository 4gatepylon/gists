class Solution {
private:
    vector<vector<int>> _subsets;
    vector<int> _subset;
    void subs(vector<int>& a, int i) {
        if (i == a.size()) {
            _subsets.push_back(vector<int>(_subset.begin(), _subset.end()));
            return;
        }
        // Try with and without
        subs(a, i + 1);
        _subset.push_back(a[i]);
        subs(a, i + 1);
        if (_subset.size() > 0) _subset.pop_back();
    }
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        _subset.clear();
        _subsets.clear();
        subs(nums, 0);
        return vector<vector<int>>(_subsets.begin(), _subsets.end());
    }
};