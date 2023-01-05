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

# O(logk)
def gcd(a: int, b: int) -> int:
    if a == b:
        return a
    if a == 1 or b == 1:
        return 1
    assert a != 0 and b != 0
    a, b = max(a, b), min(a, b)
    if a % b == 0:
        return b
    # think about it
    return gcd(b, a % b)

# O(klogk)
def gc(k: int) -> int:
    # print("*"*100) # XXX
    # print(f"K={k}") # XXX
    if k <= 2:
        return k
    acc = k
    x = 2
    xfac = 2
    xm1fac = 1
    if ((xfac + xm1fac) // k) * k == xfac + xm1fac:
        return x
    # O(k)
    while acc > 1 and x < k:
        # print(f"START x is {x} and acc is {acc} and xfac is {xfac} and xm1fac is {xm1fac}") # XXX
        xfac *= x
        xm1fac *= (x - 1)

        gcdkxm1 = gcd(acc, x - 1)
        assert (k // gcdkxm1) * gcdkxm1 == k and (xm1fac // gcdkxm1) * gcdkxm1 == xm1fac and gcdkxm1 >= 1
        acc //= gcdkxm1

        x += 1
        # print(f"END x is {x} and acc is {acc} and xfac is {xfac} and xm1fac is {xm1fac}") # XXX
    if x == k:
        return -1
    assert ((xfac + xm1fac) // k) * k == xfac + xm1fac
    assert 1 <= x and x < k
    return  k - 1 # wanted greatest one, oops

T = int(input())
for _ in range(T):
    # For each test case get the first line as n and m if necessary (we don't have to
    # because our language gets the lengths fortunately)
    # and then get the lines for a and b
    #
    k = int(input().strip())
    print(gc(k))