#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

using namespace std;
// Compile with
/*
g++ F.cc && cat F.txt | ./a.out
*/

int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    // Should always be possible to just cut the graph along some line and then make one company operate both halves, but
    // the other company operate the connectors between. This works (with k = 2) because the graph is a bipartite type thing
    // (i.e. using the connector edges you are unable to go from any node to any node on the same side, so the 1st condition
    // is met, while using the non-connector edges you can never cross so it is also met for that company; for the second
    // condition, the two companies are covering of the graph, so it should work)
    //
    // It is assumed that there is a single connected component and that this is an undirected graph.
    // To 
    for (int i = 0; i < num_tests; i++) {
        int n = -1; int m = -1;
        cin >> n >> m;
        assert(n >= 0 && m >= 0);

        // We have on edge case where everything fails: fully connected graph of size 3
        // These below are nec. and suff. conditions also this works and is a hack but whatever we need
        if (n == 3 && m == 3) {
            cout << "1 2 3" << endl;
            continue;
        }

        // n is small! so we can be very slow in n... not exponential but maybe exp(sqrt)

        // We have an O(m) adjacency vector of unordered sets which says for each node
        // what edges it's connected it. Each edge recieves an ID. Then we will also have
        // a vector of the edges that tells us for each edge what the two nodes are, and
        // a vector of the edges that tells us for each edge what the owner of that edge is.

        // Note that the errors are ok for the 0'th element and we can ignore that
        vector<unordered_set<int> > adjacency_edge(n + 1, unordered_set<int>(0)); // empty set is an error later on
        vector<unordered_set<int> > adjacency_node(n + 1, unordered_set<int>(0)); // empty set is an error later on
        vector<int> owners(m + 1, -1); // -1 represnets owner not set and should be an error
        vector<pair<int, int> > nodes(m + 1, make_pair(-1, -1)); // -1 represents node not set and should be an error
        
        // Load in the edges from a to b and vice versa (unconnected graph on [n])
        // For now 1 means that it is operated by company 1
        // We will operate with companies 1 and 2
        for (int j = 1; j <= m; j++) {
            int a = -1; int b = -1;
            cin >> a >> b;
            assert(a >= 1 && a <= n);
            assert(b >= 1 && b <= n);
            assert(a != b);

            adjacency_edge[a].insert(j); // insert into a this edge with its identity
            adjacency_edge[b].insert(j); // insert into b this edge with its identity because it's bidirectional

            // Used to deal with problem cases
            adjacency_node[a].insert(b); // insert into a this node with its identity
            adjacency_node[b].insert(a); // insert into b this node with its identity because it's bidirectional

            owners[j] = 1; // default to 1st owner before we split in half
            nodes[j] = make_pair(min(a, b), max(a, b)); // sort for convenience (probably won't use ngl)
        }

        // Find an arbitrary cut s.t.
        // 1. Each side has at least 1 node
        // 2. There is at least one edge across the cut (so the connector company will get it)
        // 3. At least one wide of the cut has at least one edge (so the island company will get it)
        
        // Easy strategy:
        // Find a node s.t. that node has at least one edge (and it does not have all the edges)
        // (such a node is guarranteed to exist because if any node has all the edges then
        // any of its neighbors cannot have all the edges assuming there are more than 2 nodes in the graph)
        // then assign that node's edges to the connector company and the rest to the island company

        // Correction: this shit will fail specifically when your selected node can reach all the other nodes (i.e. we need
        // it to be such that the node we select cannot tree the entire nodes). To solve this we can create sets for other
        // nodes of all the nodes that they can reach. Either there exists such a node or this is a complete graph.
        // We know how to solve the complete graph (or any graph that has all nodes w/ this prop.):
        // we can just put two arbitrary nodes on one side (an all the edges between them).

        int connector_nodes[2] = {1, -1};
        while (adjacency_node[connector_nodes[0]].size() == n && connector_nodes[0] <= n) {
            connector_nodes[0]++;
        }
        if (connector_nodes[0] == n && adjacency_node[connector_nodes[0]].size() == n) {
            // We have a complete graph and need to put two arbitrary nodes in one side and turn all edges
            // between them and the outside world into the connector
            connector_nodes[1] = 1;
            assert(m == n * (n - 1) / 2);
        }
        assert(adjacency_node[connector_nodes[0]].size() < n);
        for (int connector_node : connector_nodes) {
            if (connector_node != -1) {
                for (int j : adjacency_edge[connector_node]) {
                    assert(j <= m && j >= 1);

                    // Only set to company 2 if it's going out from that pair of nodes
                    int neighbor_node = nodes[j].first == connector_node ? nodes[j].second : nodes[j].first;
                    if (neighbor_node != connector_nodes[0] && neighbor_node != connector_nodes[1]) {
                        owners[j] = 2;
                    }
                }
            }
        }

        // Print our output
        cout << 2 << endl;
        for (int j = 1; j <= m; j++) {
            cout << owners[j] << (j < m ? " " : "");
        }
        cout << endl;
    }
    return 0;
}