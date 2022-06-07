# -- TEMPLATE --
import os
import sys
from atexit import register
from io import BytesIO
sys.stdout = BytesIO()
register(lambda: os.write(1, sys.stdout.getvalue()))
input = BytesIO(os.read(0, os.fstat(0).st_size)).readline
print = sys.stdout.write
# -- END TEMPLATE --

# nCk using zero indexxing
def choose(n, k):
    # n must be positive
    if n <= 0:
        return []
    # Can't pick more than n things
    elif k > n:
        return []
    # One way to pick the empty subset (this is a necessary base case or you bug)
    elif k == 0:
        return [[]]
    # One way to pick the full subset
    elif k == n:
        return [[i for i in range(n)]]
    # Otherwise, for the last elem, figure out what it would be with and without it
    # and then combine them. This is like pascal's triangle.
    else:
        without_n = choose(n - 1, k)
        with_n = choose(n - 1, k - 1)
        for sub in with_n:
            sub.append(n - 1)
        return without_n + with_n

# TODO not finished!
T = int(input())
for _ in range(T):
    # Get the graph size and subset size
    N, K = tuple(map(int, input().strip(b"\n").split(b" ")))

    # Load the graph
    adj = [[None for _ in range(N)] for _ in range(N)]
    for _ in range(N - 1):
        u, v = tuple(map(int, input().strip(b"\n").split(b" ")))
        # Change this because we're using 0-indexing
        u -= 1
        v -= 1
        adj[u][v] = 1
        adj[v][u] = 1
    
    # Topological order (where higher is further from the root) of the
    # nodes when you traverse them starting at some root
    top = [[None for _ in range(N)] for _ in range(N)]
    for r in range(N):
        # Use BFS since we care about shortest path (though either should work)
        q = [None for _ in range(N)]
        q[0] = (r, 0)
        head = 0
        while head < N and not (q[head] is None):
            # Get the next element to visit
            u, dist = q[head]

            # Update it in the topological sort map
            top[r][u] = dist

            # Increment and queue
            head += 1
            tail = head
            for v in range(N):
                # Only include elements that are both children and have not already been visited
                if not (adj[u][v] is None) and (top[r][v] is None):
                    q[tail] = (v, dist + 1)
                    tail += 1
        # Ideally we have now filled up top[r] with a mapping from node to distance from that root

    
    # TODO for every r for every subset (need a way of getting all subsets)
    # Need to find a way to get the minimum tree that spans that subset (in that topological ordering)
    # and then get its size
    for r in range(N):
        t = top[r]
        # There has to be a better way to do this!
        for sub in choose(N, K):
            visited = [0 for _ in range(N)]
            for i in sub:
                visited[i] = 1
            # TODO visit using a sort of BFS fixed point algorithm...

    print(bytes(f"TODO\n", "utf-8"))