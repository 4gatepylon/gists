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
    // Store the levels we have visited so that we can't
    // add that twice
    vector<int> view;
    unordered_set<int> levels_visited;
    void dfs(TreeNode* root, int level) {
        if (root == nullptr) return;
        if (levels_visited.find(level) == levels_visited.end()) {
            view.push_back(root->val);
            levels_visited.insert(level);
        }
        // Right has to come first for us to get the right view
        dfs(root->right, level + 1);
        dfs(root->left, level + 1);
    }
public:
    vector<int> rightSideView(TreeNode* root) {
        view.clear();
        levels_visited.clear();
        dfs(root, 0);
        return view;
    }
};