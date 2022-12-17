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
    vector<vector<int>> ps(TreeNode* root, int targetSum) {
        if (root == nullptr) return vector<vector<int>>();
        if (root->left == nullptr && root->right == nullptr) {
            // Only return an empty list to fill if we were able to consume the target sum
            if (targetSum == root->val) return vector<vector<int>>(1, vector<int>{root->val});
            else                        return vector<vector<int>>();
        }
        vector<vector<int>> left_sums = ps(root->left, targetSum - root->val);
        vector<vector<int>> right_sums = ps(root->right, targetSum - root->val);
        for (vector<int>& v : left_sums) v.push_back(root->val);
        while (right_sums.size() > 0) {
            left_sums.push_back(right_sums[right_sums.size() - 1]);
            right_sums.pop_back();

            left_sums[left_sums.size() - 1].push_back(root->val);
        }
        return left_sums;
    }
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        if (root == nullptr) return vector<vector<int>>();

        vector<vector<int>> p = ps(root, targetSum);
        for (vector<int>& v : p) {
            reverse(v.begin(), v.end());
        }
        return p;
    }
};