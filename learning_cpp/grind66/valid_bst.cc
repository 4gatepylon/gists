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
#include <bits/stdc++.h>
class Solution {
private:
    bool valid(TreeNode* root, long lo, long hi) {
        if (root == nullptr) return true;
        return lo < root->val && root->val < hi && valid(root->left, lo, root->val) && valid(root->right, root->val, hi);
    }
public:
    bool isValidBST(TreeNode* root) {
        if (root == nullptr) return true;
        return valid(root, (long)INT_MIN - 1l, (long)INT_MAX + 1l);
    }
};