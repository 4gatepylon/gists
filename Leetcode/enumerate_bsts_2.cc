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
    // Unvisited should NOT ever be a pointer or something is deeply wrong
    #define UNVISITED (vector<TreeNode*>)1
    #define MAX_NUM_TREES 10
    #define NULLPTR_VECTOR vector<TreeNode*>(1, nullptr)
    vector<TreeNode*> templates[MAX_NUM_TREES][MAX_NUM_TREES];
    TreeNode* deepcopy(TreeNode* root) {
        if (root == nullptr) return nullptr;
        TreeNode* left = deepcopy(root->left);
        TreeNode* right = deepcopy(root->right);
        return new TreeNode(root->val, left, right);
    }
    vector<TreeNode*> deepcopy(vector<TreeNode*>& trees) {
        vector<TreeNode*> treescopy;
        for (TreeNode* t : trees) treescopy.push_back(deepcopy(t));
        return treescopy;
    }
    // Create the trees recursively
    vector<TreeNode*> gt(int i, int j) {
        assert(1 <= i && i <= j);
        if (i == j) return NULLPTR_VECTOR;
        if (templates[i][j] != UNVISITED) return deepcopy(templates[i][j]);
        vector<TreeNode*> trees;
        for (int k = i; k < j; k++) {
            vector<TreeNode*> left_children = gt(i, k);
            vector<TreeNode*> right_children = gt(k + 1, j);
            for (TreeNode* left_child : left_children) {
                for (TreeNode* right_child : right_children) {
                    trees.push_back(new TreeNode(k, left_child, right_child));
                }
            }
        }
        templates[i][j] = trees;
        return trees;
    }
public:
    vector<TreeNode*> generateTrees(int n) {
        for (int i = 0; i < MAX_NUM_TREES; i++) { for (int j = 0; j < MAX_NUM_TREES; j++) templates[i][j] = UNVISITED; }
        return gt(1, n+1);
    }
};