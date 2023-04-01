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

T = int(input())
for _ in range(T):
    N = int(input())
    ns = list(map(int, input().split(b" ")))

    permutation = [i for i in range(1, N+1)]
    solved = [False] * N

    # Try and make sure no one gets their shoes AND they all get a size they can use
    # (NOTE: you can't trade up, because then your friend would trade down... I think)
    # NOTE no student here can swap back and get their shoes because you always move up
    # (and you only swap with equal sizes)
    for i in range(1, N):
        if ns[i] == ns[i - 1]:
            permutation[i], permutation[i - 1] = permutation[i - 1], permutation[i]
            solved[i], solved[i - 1] = True, True
    if all(solved):
        pstr = " ".join(map(str, permutation))
        print(bytes(f"{pstr}\n", "utf-8"))
    else:
        print(bytes("-1\n", "utf-8"))