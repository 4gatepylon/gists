/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
private:
    vector<vector<int>> lvls;
    void dfs(TreeNode* n, int d) {
        if (n == nullptr) return;
        if (lvls.size() <= d) {
            lvls.insert(lvls.end(), 1 + d - lvls.size(), vector<int>(0));
        }
        lvls[d].push_back(n->val);
        dfs(n->left, d + 1);
        dfs(n->right, d + 1);
    }
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        lvls.clear();
        dfs(root, 0);
        return lvls;
    }
};