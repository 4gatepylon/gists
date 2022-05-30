#include <stdio.h>  // printf
#include <stdlib.h> // free, malloc
#include <string.h> // memset
#include <assert.h> // assert

#ifndef bool
#define bool int
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

// We use some inline macros in our code
// Check `https://stackoverflow.com/questions/17349387/scope-of-macros-in-c`

// NOTE, this problem corresponds to a specific problem statement in Codeforces
// This is from `https://codeforces.com/gym/103741/problem/A`

// Static Graph Data Structure

// Can be optimized in a few ways, potentially
// 0. Cache across queries (i.e. what some queries give you, might help you later, esp.
//    if we use a DP-style solution).
// 1. Make it more cache friendly (look at the order of the edges and vertices primarily)
// 2. Make it more parallelizeable
// 3. Make it take up less memory (which affects 1)
struct GraphSt {
    int N;
    int M;
    // Length N (indexxes into `edges`, singletons correspond to vertices)
    int* offsets;
    // Length M (indexxed into by `offsets`, segments correspond to edges)
    int* edges;
};

struct EdgeSt {
    int from;
    int to;
};

typedef struct GraphSt Graph;
typedef struct EdgeSt Edge;

Graph* make_graph(int N, int M, Edge* edges) {
    Graph* g = malloc(sizeof(Graph));
    g->N = N;
    g->M = M;
    g->offsets = malloc(sizeof(int) * N);
    g->edges = malloc(sizeof(int) * (2 * M + N));

    // Count the number of edges for each vertex
    int* counts = malloc(sizeof(int) * N);

    memset(counts, 0, sizeof(int) * N);
    for (int i = 0; i < M; i++) {
        int from = edges[i].from;
        int to = edges[i].to;

        // Mandatory sanity test, can be removed with compiler flags (check below)
        assert(1 <= from && from <= N);
        assert(1 <= to && to <= N);

        counts[from - 1] ++;
        counts[to - 1] ++;
    }

    // Set to -1 for debugging discoverability
    memset(g->edges, -1, sizeof(int) * (2 * M + N));
    memset(g->offsets, -1, sizeof(int) * N);

    // Set the correct offset, put the count (number of neighbor edges) in the first element,
    // then set the offset to the last index that is insertable into
    int prev_count = 0;
    int curr_count = counts[0];
    int prev_end_offset = -1;
    int curr_offset = prev_end_offset + 1;
    g->offsets[0] = curr_count;
    g->edges[curr_offset] = curr_count;
    for (int i = 1; i < N; i++) {
        prev_count = counts[i - 1];
        curr_count = counts[i];
        prev_end_offset = g->offsets[i - 1];
        curr_offset = prev_end_offset + 1;

        g->edges[curr_offset] = curr_count;
        // Remember to set to the end offset (i.e. last element in the list of nodes)
        g->offsets[i] = curr_offset + curr_count;
    }

    // Insert all the edges into the graph
    for (int i = 0; i < M; i++) {
        int from = edges[i].from;
        int to = edges[i].to;

        // Mandatory sanity test, can be removed with compiler flags (check below)
        assert(1 <= from && from <= N);
        assert(1 <= to && to <= N);

        int from_offset = g->offsets[from - 1];
        int to_offset = g->offsets[to - 1];

        // Insert and go to the next free spot
        g->edges[from_offset] = to;
        g->edges[to_offset] = from;
        g->offsets[from - 1] --;
        g->offsets[to - 1] --;
    }

    // Check that for the very first case the offsets decremented correctly
    assert(g->offsets[0] == 0);

    free(counts);

    return g;
}

void print_graph(Graph* g) {
    printf("Graph with %d vertices and %d edges\n", g->N, g->M);
    for (int i = 0; i < g->N; i++) {
        int count = g->edges[g->offsets[i]];
        printf("Vertex %d has %d edges: ", i + 1, count);
        for (int j = 1; j < count; j++) {
            int to = g->edges[g->offsets[i] + j];
            printf("%d ", to);
        }
        if (count > 1) {
            int to = g->edges[g->offsets[i] + count];
            printf("%d\n", to);
        }
    }
    if (g->N == 0) {
        printf("\n");
    }
    printf("\n");
}

void free_graph(Graph* g) {
    free(g->offsets);
    free(g->edges);
    free(g);
}

// Store the next node 
struct JumpSt {
    int next_x;
    int next_y;
    int overlaps;
};
typedef struct JumpSt Jump;

int min_common(int u, int v, int x, int y, Graph* graph) {
    // NOTE toggle off RUN_MIN_COMMON to return 0 for all cases
    // (useful for debugging the graph creation process using the
    // same testing functionality below in `test1(...)`, etc...)
    // #define RUN_MIN_COMMON
    #ifdef RUN_MIN_COMMON
    // Number of vertices and number of edges, respectively
    int N = graph->N;
    int M = graph->M;

    // Store a table of the number of overlapping edges on a path from a given node pair
    // to the final node pair (x, y). Set it to N + 1 if it does not exist, since all paths
    // should be shorter than N + 1 if they have the minimum number of overlaps (otherwise
    // they would at best overlap an equal number of times).
    Jump* jumps = malloc(sizeof(Jump) * N * N);

    // J queries the jump table and EEQ checks if two edges are equal
    #define J(a, b) (jumps[(a) * (N) + (b)])
    #define EEQ(a, b, c, d) ((a == c && b == d) || (a == d && b == c))

    // Initialize the jump table (both nexts are -1 if there is no next)
    for (int u = 0; u < N; u++) {
        for (int v = 0; v < N; v++) {
            J(u, v).overlaps = N + 1;
            J(u, v).next_x = -1;
            J(u, v).next_y = -1;
        }
    }

    // Fixed point DP algorithm implementation in <= O((MN)^2 N)
    // based on the obervation that
    // 1.   For every pair of node-pairs, the minimum overlapping pair of paths is through
    //      a sequence of two node pairs such that for each we are taking the minimum overlap
    //      (and that sequence can simply be the first step from the starter pair).
    //
    // 2.1  There are often multiple solutions.
    // 2.2  The optimal pair of paths for the node pair (i.e. with the minimal overlap) is
    //      of length up to N, because otherwise there is a loop, and if there is a loop then
    //      there exists a shorter path (i.e. without a loop), which by virtue of being a prefix
    //      of the looped version, cannot possibly be more suboptimal.
    for (int _i = 0; _i < graph->N; _i++) {
        // Go through every pair of vertices and for every combination of edges they have
        // set the minimum number of overlaps, called `overlaps`, (and `next`) to correspond
        // to the pair of nodes that eminate from taking that combination (pair) of edges
        // that minimizes the overlaps. If the edges taken to those nodes are 0 then the overlaps
        // increments by zero with respect to the partial solution of that pair of nodes,
        // otherwise it increments by 1.
        for (int u = 0; u < N; u++) {
            for (int v = 0; v < N; v++) {
                // Find the segment that corresponds to the edges for this node (i.e. neighbors).
                int u_offset                 = graph->offsets[u];
                int v_offset                 = graph->offsets[v];
                int u_num_edges              = graph->edges[u_offset];
                int v_num_edges              = graph->edges[v_offset];

                u_offset ++;
                v_offset ++;
                for (int u_neighbor_idx = u_offset; u_neighbor_idx < u_offset + u_num_edges; u_neighbor_idx ++) {
                    for (int v_neighbor_idx  = v_offset; v_neighbor_idx < v_offset + v_num_edges; v_neighbor_idx ++) {
                        int u_neighbor       = graph->edges[u_neighbor_idx];
                        int v_neighbor       = graph->edges[v_neighbor_idx];

                        // Neighbors overlaps ( +1 if the edge is the same )
                        int overlaps         = J(u_neighbor, v_neighbor).overlaps + EEQ(u, u_neighbor, v, v_neighbor);

                        // Update the jump table if we find a better solution
                        // (with less overlaps)
                        if (overlaps < J(u, v).overlaps) {
                            J(u, v).overlaps = overlaps;
                            J(u, v).next_x   = u_neighbor;
                            J(u, v).next_y   = v_neighbor;
                        }

                    }
                }
            }
        }
    }

    free(jumps);

    // Sanity test that we did in fact solve this solution (since according to the problem
    // statement the graph was `connected`, which I also take to mean it was connected BETWEEN
    // THOSE PARTICULAR NODES)
    assert(J(u, v).overlaps < N + 1);
    assert(J(u, v).next_x != -1);
    assert(J(u, v).next_y != -1);
    return J(u, v).overlaps;

    #undef J

    #else
    // Dummy to debug graph creation
    // NOTE that this corresponds to the ifdef of `RUN_MIN_COMMON` above
    return 0;
    #endif
}

bool test1(int print_only /*Whether to actually fail or only print the outputs.*/) {
    printf("-- TEST 1 --\n\n");

    // Input
    // -- N and M (number of vertices and edges respectively)
    // 7 8
    // -- Edges
    // 1 2
    // 2 3
    // 3 1
    // 3 4
    // 4 5
    // 5 6
    // 6 7
    // 7 4
    // -- Number of Queries, called Q
    // 4
    // -- Queries
    // 1 1 3 3
    // 1 2 5 6
    // 1 5 6 2
    // 4 5 6 7

    #define N 7
    #define M 8
    Graph* g = make_graph(N, M, (Edge[M]) {
        {1, 2},
        {2, 3},
        {3, 1},
        {3, 4},
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},
    });

    // Print for debugging
    print_graph(g);

    #define Q 4
    int queries[Q][4] = {
        {1, 1, 3, 3},
        {1, 2, 5, 6},
        {1, 5, 6, 2},
        {4, 5, 6, 7},
    };

    // Desired output is 
    // 0
    // 1
    // 0
    // 0
    int expected_output[Q] = {
        0,
        1,
        0,
        0,
    };

    int outputs[Q] = {
        -1,
        -1,
        -1,
        -1,
    };

    // Display for debugging
    for (int i = 0; i < Q; i++) {
        int u = queries[i][0];
        int v = queries[i][1];
        int x = queries[i][2];
        int y = queries[i][3];
        outputs[i] = min_common(u, v, x, y, g);

        printf("Query %d/%d, (%d, %d) -> (%d, %d) = %d (should = %d)\n", i, Q, u, v, x, y, outputs[i], expected_output[i]);
    }

    // Check for error handling
    for (int i = 0; i < Q; i++) {
        assert(outputs[i] != -1);
        assert(print_only || outputs[i] == expected_output[i]);
    }

    free_graph(g);

    printf("\n\n-- END TEST 1 --\n\n");
    return true;

    // So that we can avoid preprocessor warnings (since each test function inlines these definitions)
    #undef N
    #undef M
    #undef Q
}

bool test2(int print_only /*Whether to actually fail or only print the outputs.*/) {
    printf("-- TEST 2 --\n\n");
    // Input
    // -- N and M (number of vertices and edges respectively)
    // 8 7
    // -- Edges
    // 1 2
    // 2 3
    // 3 4
    // 3 5
    // 1 6
    // 6 7
    // 6 8
    // -- Number of Queries, called Q
    // 3
    // -- Queries
    // 4 5 7 8
    // 4 3 2 1
    // 5 5 8 8
    #define N 8
    #define M 7
    Graph* g = make_graph(N, M, (Edge[M]) {
        {1, 2},
        {2, 3},
        {3, 4},
        {3, 5},
        {1, 6},
        {6, 7},
        {6, 8},
    });

    // Print for debugging
    print_graph(g);

    #define Q 3
    int queries[Q][4] = {
        {4, 5, 7, 8},
        {4, 3, 2, 1},
        {5, 5, 8, 8},
    };

    // Desired output is
    // 3
    // 1
    // 5
    int expected_output[Q] = {
        3,
        1,
        5,
    };

    int outputs[Q] = {
        -1,
        -1,
        -1,
    };

    // Display for debugging
    for (int i = 0; i < Q; i++) {
        int u = queries[i][0];
        int v = queries[i][1];
        int x = queries[i][2];
        int y = queries[i][3];
        outputs[i] = min_common(u, v, x, y, g);

        printf("Query %d/%d, (%d, %d) -> (%d, %d) = %d (should = %d)\n", i, Q, u, v, x, y, outputs[i], expected_output[i]);
    }

    // Check for error handling
    for (int i = 0; i < Q; i++) {
        assert(outputs[i] != -1);
        assert(print_only || outputs[i] == expected_output[i]);
    }

    free_graph(g);

    printf("\n\n-- END TEST 2 --\n\n");
    return true;

    // So that we can avoid preprocessor warnings (since each test function inlines these definitions)
    #undef N
    #undef M
    #undef Q
}

int main() {
    // Toggle `PRINT_ONLY` on to debug the graph creation process.
    #define PRINT_ONLY true

    assert(test1(PRINT_ONLY));
    assert(test2(PRINT_ONLY));

    #undef PRINT_ONLY
}