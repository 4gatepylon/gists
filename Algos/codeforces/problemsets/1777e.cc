#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>
#include "assert.h"

using namespace std;

// Compile with
/*
g++ 1772e.cc && cat 1772e.txt | ./a.out
*/

// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
int main() {
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    for (int i = 0; i < num_tests; i++) {
        int n = -1;
        cin >> n;
        assert(n >= 1);

        int m = -1;
        cin >> m;
        assert(m >= 1);

        // Did not figure a sufficiently fast algorithm for this in time but the idea I came up with is
        // O(n^3logn) and basically the steps are these
        // 0. Check how many weakly connected components there are. If there are more then one then
        // simply finish early and return -1. This is done using a simple BFS or DFS.
        // 1. Run a connected components graph to create a list of the connected components (sets).
        // 2. For each node find which connected component it is in, by iterating over connected components and
        // (after giving each component an ID) assigning that ID as the component of that node in a map.
        // 3. For each edge look at the incoming and outgoing vertices. If they are not in the same connected
        // component (in the map from 2) then connect those two connected components with that edge. Note that
        // if those two connected components were already connected, then we can simply set the cost of the
        // pre-existing edge to the minimum of its previous cost and the cost of this edge we are looking at.
        // Note: we are creating a new graph in this step (step 3).
        // 
        // Note: our new graph should now be a DAG and it should be one connected component (weakly) still.
        // We can prove that in this graph (or any DAG for that matter) the solution is to pick any one of the
        // sources (i.e. 0'th elements, or those without any parents or in-arrows) as our node to point to all
        // others.
        //
        // In this regime we iterate over all the sources and for each source we run a modified version of Djikstra's.
        // It works like this: we first clone the graph. In the cloned graph we run BFS from the source node and for all
        // edges on the path from our chosen source we set the weight to zero if we can BFS in the directed direction.
        // Then, we run djikstra's (finding the path) from the source node to the first other node (that is also
        // a topological source). For all vertices on that path we set their weight to zero. Then we repeat on this
        // modified graph, doing djikstra's from that original node to the second and then so forth. Eventually,
        // we've run djikstra's from from the source node to all the other sources, and thus we have reached the entire
        // graph. The sum of the distances is our solution.
        //
        // NOTE: I'm not entirely sure that the last thing here works....
        //
        // This problem seemed very easy but I was a little bit stumped.
        //
        //
        // NOTE: I misread the question, it's the MAXIMUM OF THE REVERSED EDGES
        // ANSWER: use binary search on the minimum cost and then do BFS with edges <= c
        // as bidirectional. We can find in linear time if a node that is a source exists,
        // because we can use toposort and check if there exists only one source.
        //
        // ... feel kinda bad about this one
    }
}