/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if (root == nullptr) return "{}";
        string left = serialize(root->left);
        string right = serialize(root->right);
        string mid = std::to_string(root->val);

        std::ostringstream stream;
        stream << "{" << mid << "" << left << right << "}";
        return stream.str();
    }
    pair<TreeNode*, int> des(string data, int i) {
        assert(data[i] == '{');
        if (data[i+1] == '}') {
            return pair<TreeNode*, int>(nullptr, i + 2);
        }

        int start_left = i + 1;
        int val = 0;
        int op = 1;
        if (data[start_left] == '-') {
            op = -1;
            start_left++;
        }
        while (data[start_left] != '{') {
            val *= 10;
            val += data[start_left] - '0';
            start_left++;
        }
        val *= op;
        pair<TreeNode*, int> leftp = des(data, start_left);
        pair<TreeNode*, int> rightp = des(data, leftp.second);
        assert(data[rightp.second] == '}');

        TreeNode* t = new TreeNode(val);
        t->left = leftp.first;
        t->right = rightp.first;
        return pair<TreeNode*, int>(t, rightp.second + 1);
    }
    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        return des(data, 0).first;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));