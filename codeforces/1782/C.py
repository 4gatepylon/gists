from __future__ import annotations

import math

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

# Index in a frequency array to count the number of times a letter appears
def idx(a: str) -> int:
    assert len(a) == 1
    assert ord('a') <= ord(a) and ord(a) <= ord('z')
    return ord(a) - ord('a')

# Build a sorted frequency map including characters that don't show up but
# are in our space of characters
def build_freq_arr(s: str) -> list[int]:
    freq = [0] * 26
    for c in s:
        freq[idx(c)] += 1
    return freq

def find_divisors(n: int) -> list[int]:
    ds = []
    for i in range(1, math.ceil(math.sqrt(n))):
        if n % i == 0:
            ds.append(i)
    for i in range(len(ds) - 1, -1, -1):
        ds.append(n // ds[i])
    assert len(ds) == len(set(ds))
    assert ds == sorted(ds)
    return ds

# Get an arbitrary character from the dictionary
def arb_char(chars: dict[str, int]) -> str:
        for c, f in chars.items():
            assert f > 0
            chars[c] -= 1
            if chars[c] == 0:
                del chars[c]
            return c
        raise Exception("No character found")

# If it is above 
def cost_func_and_str(freq: list[int], div: int, s: str) -> int:
    assert sum(freq) % div == 0
    assert len(freq) == 26

    target_num_chars = sum(freq) // div
    actual_num_chars = sum((1 for x in freq if x != 0))
    
    # Decide what characters to have in histo
    histo_all = sorted([(chr(ord('a') + i), freq[i]) for i in range(26)], key=lambda x: x[1])
    histo = [(c, f) for c, f in histo_all if f != 0]
    if target_num_chars > actual_num_chars:
        histo = [(c, 0) for c, f in histo_all if f == 0][:target_num_chars - actual_num_chars] + histo
    elif target_num_chars < actual_num_chars:
        # If we need to delete characters we delete the less often ones first is what we discovered
        histo = histo[len(histo) - target_num_chars:]
    # print(histo)
    assert len(histo) == target_num_chars
    assert histo == sorted(histo, key=lambda x: x[1])

    # We will count every cost twice, once for the removal and once for the insertion
    # However, if we need to delete characters, then we will count the insertion
    cost = 0
    for _, f in histo:
        cost += abs(f - div)
    
    assert cost % 2 == 0 or target_num_chars < actual_num_chars, f"Cost is not even {cost} for div {div} on string {s}"
    cost = cost // 2 if target_num_chars >= actual_num_chars else cost
    
    # generate the string, every element in histo will have the counts
    chars = {c : div for c, _ in histo}
    # print(chars)
    generated_string = ''
    for c in s:
        if c in chars:
            generated_string += c
            chars[c] -= 1
            if chars[c] == 0:
                del chars[c]
        else:
            generated_string += '0'
    # print(f"div is {div} and cost was {cost} and from s {s} we got {generated_string}") # XXX
    generated_string = ''.join((c if c != '0' else arb_char(chars) for c in generated_string))
    return cost, generated_string

# We can solve by first finding the frequency map, then
# changing the frequency map to have the frequencies we want,
# then iterating through the string and greedily replacing
# replacing with characters (replacing when we run out)
def solve(s: str) -> str:
    freq = build_freq_arr(s)
    divs = find_divisors(len(s))
    # print(divs)
    # print(cost_func_and_str(freq, divs[0], s))
    # print(cost_func_and_str(freq, divs[1], s))
    costs_and_strs = [cost_func_and_str(freq, div, s) for div in divs]
    return min(costs_and_strs, key=lambda x: x[0])[1]

T = int(input())
for _ in range(T):
    input()
    string = input().decode('utf-8').strip()
    print(solve(string))

# Didn't finish in time fk