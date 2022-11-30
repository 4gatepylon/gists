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
    unordered_map<TreeNode*, int>size;
    int fill_size(TreeNode* root) {
        if (root == nullptr) return 0;
        int s = 1 + fill_size(root->left) + fill_size(root->right);
        size[root] = s;
        return s;
    }
    int kth_smallest(TreeNode* root, int k) {
        assert(root != nullptr);

        int left_size = root->left == nullptr ? 0 : size[root->left];
        if (left_size == k - 1) return root->val;
        else if (left_size >= k) return kth_smallest(root->left, k);    
        else return kth_smallest(root->right, k - 1 - left_size);
    }
public:
    int kthSmallest(TreeNode* root, int k) {
        size.clear();
        fill_size(root);
        return kth_smallest(root, k);
    }
};