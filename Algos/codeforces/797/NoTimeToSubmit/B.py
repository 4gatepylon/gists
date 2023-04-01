# Expected Default Test Output:
# YES
# YES
# NO
# NO
# YES
# NO

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

def num_ops(N, source, target):
    # "None" diff signifies any diff is OK
    diff = None
    for i in range(N):
        # Because the operation is idempotent after zero, we only use differences of non-zero numbers
        # (they must all be the same)
        if target[i] != 0:
            # If any diff is OK then select the one that is forced by this number
            # Else fail if the diff does not match one which we found was necessary before
            if diff is None:
                diff = source[i] - target[i]
            elif diff != source[i] - target[i]:
                return False
            
            # You can only go down, so if you need to go up anywhere it's impossible
            if diff < 0:
                return False
    return True
            

T = int(input())
for _ in range(T):
    N = int(input())
    source = list(map(int, input().strip(b"\n").split(b" ")))
    target = list(map(int, input().strip(b"\n").split(b" ")))
    assert(len(source) == N)
    assert(len(target) == N)
    O = num_ops(N, source, target)
    print(bytes("YES\n" if O else "NO\n", "utf-8"))