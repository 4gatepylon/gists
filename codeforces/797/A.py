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
        return (n // 3 - 1) + 2
    elif n % 3 == 1:
        # (n // 3 - 1) pf width 3
        # 1 of width 2
        # 2 of widh 1
        return (n // 3 - 1) + 3
    else:
        # -- n % 3 == 2
        # (n // 3 - 1) of width 3
        # 2 of width 2
        # 1 of width 1
        return (n // 3 - 1) + 3

T = int(input())
for _ in range(T):
    N = int(input())
    H = pedestal_height(N)
    print(bytes(f"{H}\n", "utf-8"))