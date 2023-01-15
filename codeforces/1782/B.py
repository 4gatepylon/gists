from __future__ import annotations

# -- TEMPLATE --
import os
import sys
from atexit import register
from io import BytesIO
sys.stdout = BytesIO()
# THIS WILL MAKE IT PRINT EVERYTHING ALL AT ONCE IN THE END
register(lambda: os.write(1, sys.stdout.getvalue()))
input = BytesIO(os.read(0, os.fstat(0).st_size)).readline
print = lambda x: sys.stdout.write(bytes(f"{x}\n", "utf-8"))
# -- END TEMPLATE --

def count(a: list[int]) -> int:
    a = sorted(a)
    assert a[0] >= 0

    # If everyone needs to be with someone else, then we can take zero people, otherwise
    # it is impossible to take zero people
    c = 0
    if a[0] != 0:
        c += 1
    
    including = False
    for i in range(len(a)):
        # Imagine we include person i, then we will have i people, we should add
        # one to the count c if the person's count is under i, however, we need to only count people
        # such that they are the last to be included (basically overflow until we cannot anymore,
        # poetically speaking)
        if not including and a[i] <= i:
            # Start overflowing
            including = True
        elif including and a[i] > i:
            # Stop overflowing
            c += 1
            including = False
    if including:
        c += 1
    return c

T = int(input())
for _ in range(T):
    input()
    a = list(map(int, filter(lambda x: len(x) > 0, input().split())))
    print(count(a))