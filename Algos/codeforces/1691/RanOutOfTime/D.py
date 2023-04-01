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

# TODO too slow!
T = int(input())
for _ in range(T):
    # Get the arrays
    N = int(input())
    ns = list(map(int, input().strip(b"\n").split(b" ")))

    # Generate a table of the maximum value for different contiguous subsequences in O(N^2)
    # M[L][i] is the maximum value of the subsequence ns[i:i+L]
    M = [[None for _ in range(N+1)] for _ in range(N+1)]
    for i in range(0, N):
        M[1][i] = ns[i]
    for L in range(2, N + 1):
        for i in range(0, N + 1 - L):
            prev_max = M[L-1][i]
            last_elem = ns[i + (L - 1)]
            M[L][i] = max(prev_max, last_elem)
        
    # Generate a sum prefix table so we can find the value of the sum
    S = [None for _ in range(N)]
    S[0] = ns[0]
    for i in range(1, N):
        S[i] = S[i-1] + ns[i]
    
    # For each subsequence, confirm check whether the condition holds (max >= sum)
    # We define each subsequence as [i, j], so j IS included!
    holds = True
    for i in range(0, N):
        for j in range(i, N):
            L = j - i + 1
            S_ij = S[j] - S[i - 1] if i > 0 else S[j]

            assert(0 <= L and L <= N)
            assert(0 <= i and i < N)
            assert(i <= j and j < N)
            if M[L][i] < S_ij:
                holds = False
                break
    
    # Format the output the way they want
    string = "YES" if holds else "NO"
    print(bytes(f"{string}\n", "utf-8"))
# ...