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
    // Given a root, try and add the i'th element
    // of preorder as a child if possible and otherwise do nothing
    // return the next index to add from (once we return, the caller is expected to
    // start adding in ANOTHER subtree)
    int fill(
        // Root and parent to tell us what to add and where to add it
        TreeNode* root,
        // Preorder and inorder to tell us what to add next
        vector<int>& preorder,
        int i,
        // Ordering to know when we are in the wrong subtree
        unordered_map<int, int>& ordering,
        int min_parent_to_right_ordering
    ) {
        if (i >= preorder.size()) return i;
        int j = i;
        if (ordering[preorder[i]] < ordering[root->val]) {
            // DEBUG
            // cout << "inserting " << preorder[i] << " at " << i << " to the left of " << root->val << "\n";
            root->left = new TreeNode(preorder[i]);
            // The parent is to the right because we are moving
            // left
            j = fill(root->left, preorder, i + 1, ordering, min(min_parent_to_right_ordering, ordering[root->val]));
        }

        if (j >= preorder.size()) return j;
        int k = j;
        if (
            // You want to go right only if there does not exist a parent
            // such that you'd want to be to THEIR right
            // There is such a parent IFF
            // 1. There is a parent with an ordering SMALLER than you
            // 2. That parent was to the RIGHT of its child which is also a parent of you
            ordering[preorder[k]] < min_parent_to_right_ordering && 
            ordering[preorder[k]] > ordering[root->val]) {
            // DEBUG
            // cout << "inserting " << preorder[j] << " at " << j << " to the right of " << root->val << "\n";
            root->right = new TreeNode(preorder[k]);
            // We are moving right, so the min parent to the right does not change
            k = fill(root->right, preorder, k + 1, ordering, min_parent_to_right_ordering);
        }
        return k;
    }
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        // inorder is sorted from left to right
        // preorder is missing nulls basically
        //   if preorder has length N then we'd like to insert 2^K - 1 - N nulls
        //   (i.e. 2^K - 1  for smallest K s.t. it's >= N) to complete the binary tree...
        //   where nulls represent empty nodes and some may be children of nulls
        //   (and every 2^i elements is a row basically)

        // Basically go through pre-order
        // 1. Insert Root
        // 2. If 2nd element smaller than previous then go left and then insert
        // 3. Else if greater, but smaller than parent insert to right
        // 4. Else go up and try again
        // O(n) since each node visited only twice at most (once down once up)
        unordered_map<int, int> ordering(inorder.size());
        for (int i = 0; i < inorder.size(); i++) { ordering[inorder[i]] = i; }

        TreeNode* root = new TreeNode(preorder[0]);
        assert(fill(root, preorder, 1, ordering, inorder.size()) == inorder.size());
        return root;
    }
};