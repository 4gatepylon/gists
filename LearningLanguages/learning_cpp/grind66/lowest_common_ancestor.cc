/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
private:
    TreeNode* a;
    pair<bool, bool> contains(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == nullptr) return pair<bool, bool>{false, false};

        // Update based on this node
        bool cp = false;
        bool cq = false;
        if (root == p) cp = true;
        if (root == q) cq = true;

        // Recursively find left and right
        pair<bool, bool> cleft = contains(root->left, p, q);
        pair<bool, bool> cright = contains(root->right, p, q);
        cp = cp || cleft.first || cright.first;
        cq = cq || cleft.second || cright.second;

        // If we've found it add the first time
        if (this->a == nullptr && cp && cq) this->a = root;

        return pair<bool, bool>{cp, cq};
    }
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        a = nullptr;
        contains(root, p, q);
        return a;
    }
};