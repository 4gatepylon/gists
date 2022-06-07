# Expected Default Test Output:
# 1
# 2
# 0
# 1

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

def min_changes(n, k, a):
    # ...
    num_white = 0

    # First segment of length k
    for i in range(0, k):
        num_white += (a[i] == "W")
    m = num_white
    # all other segments of length k
    for i in range(k, n):
        num_white = num_white - (a[i - k] == "W") + (a[i] == "W")
        m = min(m, num_white)
    
    # ...
    return m

T = int(input())

for _ in range(T):
    N, K = tuple(map(int, input().strip(b"\n").split(b" ")))
    A = list(map(str, input().strip(b"\n").split(b" ")))

    # Sanity test that this is a well-formed array
    assert (0 < N and 0 < K and K <= N)
    assert(len(A) == N)
    assert(sorted(list(set(A)), key=lambda x: 1 if x == "W" else 0) == ["B", "W"])
    
    print(bytes(f"{min_changes(N, K, A)}", "utf-8"))