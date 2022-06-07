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

# TODO not finished!
# (I solved the version where any two segments can be combined, which works
# just with a sort...)
T = int(input())
for _ in range(T):
    G = 0
    N = int(input().strip(b"\n"))
    ns = []
    for _ in range(N):
        color, left, right = tuple(map(int, input().strip(b"\n").split(b" ")))
        ns.append((color, left, right))
    # TODO figure out how to combine things here
    print(bytes(f"{G}\n", "utf-8"))