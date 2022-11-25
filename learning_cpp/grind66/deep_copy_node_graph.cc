/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};
*/

class Solution {
private:
    unordered_map<int, Node*> created;
    Node* dfs(Node* n) {
        // If we already created a node don't do it again
        if (n == nullptr) return nullptr;
        if (created.count(n->val) > 0) return created[n->val];

        // Create the new node
        Node* new_node = new Node(n->val, vector<Node*>(0));

        // Make sure we won't be re-creating it
        created[n->val] = new_node;

        // Recursively create the neighbors
        for (Node* neighbor : n->neighbors) {
            new_node->neighbors.push_back(dfs(neighbor));
        }

        // Return the new node
        return new_node;
    }
public:
    Node* cloneGraph(Node* node) {
        // Get the graph into an adj Graph
        created.clear();
        return dfs(node);
    }
};