# Expected Default Test Output:
# 2 7 1 
# 1 1 
# 1 183 1 60 7644 900 914 80152 5644467 
# 1000000000 

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

def durations(N, starts, finishes):
    # He immediately starts on the first task
    max_start_time = starts[0]
    
    # For all other tasks he starts either when he finishes the previous one
    # or idles until he gets it and can start it
    ds = []
    for i in range(N - 1):
        ds.append(finishes[i] - max_start_time)
        max_start_time = max(finishes[i], starts[i + 1])
    # The final one is the same, but we don't update max_start_time
    ds.append(finishes[-1] - max_start_time)

    # ...
    assert(len(ds) == N)
    return ds

T = int(input())
for _ in range(T):
    N = int(input())
    starts = list(map(int, input().strip(b"\n").split(b" ")))
    finishes = list(map(int, input().strip(b"\n").split(b" ")))

    assert(len(starts) > 0)
    assert(len(starts) == N)
    assert(len(starts) == len(finishes))
    print(bytes(" ".join(durations(N, starts, finishes)), "utf-8"))