# NOTE this is meant to test zeros2ones.py because
# there are unknown runtime errors...

from __future__ import annotations

import subprocess
import random

# first option is "zero" others are 1, 2, ... (basically it's a base)
def combos(options: list[str]):
    next_combo = options[0]
    while True:
        yield next_combo
        prev_combo = next_combo
        next_combo = None
        full_carry = True
        for idx in range(len(prev_combo) - 1, -1, -1):
            if prev_combo[idx] != options[-1]:
                next_combo = prev_combo[:idx] + options[options.index(prev_combo[idx])+1] + options[0]*(len(prev_combo) - idx - 1)
                full_carry = False
                break
        assert full_carry or next_combo is not None
        if full_carry:
            next_combo = options[0]*(len(prev_combo) + 1)

def rand_strs(l: int=100, deterministic: bool=False):
    if not deterministic:
        for _ in range(l):
            length = random.randint(1, 100)
            random_str = ''.join([random.choice(['0', '1']) for _ in range(length)])
            yield random_str
    else:
        for i, combo in enumerate(combos(["0", "1"])):
            if i >= l:
                break
            yield combo

def rand_xys(l: int=100):
    for _ in range(l):
        rand_x = random.randint(1, 100)
        yield rand_x

## MAIN

ERR_ONLY = False
for rand_str, rand_x, rand_y in zip(rand_strs(l=1000, deterministic=True), rand_xys(l=1000), rand_xys(l=1000)):
    print(f"len_rand_str: {len(rand_str)} rand_str: {rand_str}, rand_x: {rand_x}, rand_y: {rand_y}") # XXX
    print("-"*30)
    p = subprocess.Popen(['python3', 'zeros2ones.py'], stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.STDOUT)    
    _out, _err = p.communicate(input=bytes(f"{len(rand_str)} {rand_x} {rand_y}\n{rand_str}\n", 'utf-8'))
    had_err = p.returncode != 0
    assert had_err == (_err is not None), f"had_err: {had_err}, _err: {_err}, _err is not None {_err is not None}"

    _out = b"" if _out is None else _out
    _err = b"" if _err is None else _err
    output = _out.decode('utf-8') + "\n*** ERR *** \n" + _err.decode('utf-8')
    if not ERR_ONLY or had_err:
        print("output", output)
        print("-"*100)