# Expected Default Test Output:
# 4 5 2
# 2 3 1
# 4 5 1
# 33334 33335 33331
# 2 4 1
# 3 4 1

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

def pedestal_height(n):
    assert(n >= 6)
    if n % 3 == 0:
        # (n // 3 - 1) of width 3
        # 1 of width 2
        # 1 of width 1
        w3 = (n // 3 - 1)
        w2 = w3 + 1
        w1 = w2 + 1
        return (w3, w2, w1)
    elif n % 3 == 1:
        # (n // 3 - 1) of width 3
        # 1 of width 2
        # 2 of widh 1
        w3 = (n // 3 - 1)
        w2 = w3 + 1
        w1 = w2 + 2
        return (w3, w2, w1)
    else:
        # -- n % 3 == 2
        # (n // 3 - 1) of width 3
        # 2 of width 2
        # 1 of width 1
        w3 = (n // 3 - 1)
        w2 = w3 + 2
        w1 = w2 + 1
        return (w3, w2, w1)

T = int(input())
for _ in range(T):
    N = int(input())
    w3, w2, w1 = pedestal_height(N)
    assert(w3 + w2 + w1 == N), f"Yields w3:{w3}, w2:{w2}, w1:{w1} for {N} blocks"
    assert(w1 > w2 and w2 > w3)
    print(bytes(f"{w2} {w1} {w3}\n", "utf-8"))