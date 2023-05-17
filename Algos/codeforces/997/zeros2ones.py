# Run with `cat zeros2ones.txt | python3 zeros2ones.py`

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

def numZeroIslands(s: str) -> int:
    if len(s) == 0:
        return 0
    assert all([c in ['0', '1'] for c in s])
    single_fenced = [c for i, c in enumerate(s) if i == 0 or c == '0' or s[i-1] != '1']
    as_str = "".join(single_fenced)
    split_into_groups = as_str.split('1')
    filtered_groups = [g for g in split_into_groups if g != '']
    return len(filtered_groups)
assert numZeroIslands("1") == 0
assert numZeroIslands("0") == 1
assert numZeroIslands("000") == 1
assert numZeroIslands("0001") == 1
assert numZeroIslands("00010") == 2
assert numZeroIslands("1000") == 1
assert numZeroIslands("010001") == 2
assert numZeroIslands("0100010") == 3
assert numZeroIslands("1010001001") == 3

def costOfStr(s: str, x: int, y: int) -> int:
    if len(s) == 0:
        return 0
    
    num_zero_islands = numZeroIslands(s)
    assert num_zero_islands >= 0
    assert num_zero_islands < len(s) or len(s) == 1 and s[0] == '0'

    if num_zero_islands == 0:
        return 0
    elif x <= y:
        return x * num_zero_islands
    else:
        assert y < x
        flip_times = num_zero_islands - 1
        return (y * flip_times) + x


# NOTE that I change the semantics of x and y
# x = cost to flip
# y = cost to reverse
# goal is to turn all into 1's
n, y, x = map(lambda x: int(x.strip()), input().split())
s = input().strip().decode('utf-8')
assert type(n) == int, type(n)
assert type(x) == int, type(x)
assert type(y) == int, type(y)
assert type(s) == str, type(s)

assert len(s) == n

cost = costOfStr(s, x, y)
print(cost)