from __future__ import annotations

# -- TEMPLATE --
import os
import sys
import math
from atexit import register
from io import BytesIO
sys.stdout = BytesIO()
# THIS WILL MAKE IT PRINT EVERYTHING ALL AT ONCE IN THE END
register(lambda: os.write(1, sys.stdout.getvalue()))
input = BytesIO(os.read(0, os.fstat(0).st_size)).readline
print = lambda x: sys.stdout.write(bytes(f"{x}\n", "utf-8"))
# -- END TEMPLATE --

def rand_char(chars: dict[str, int]):
    assert len(chars) > 0
    for char, freq in chars.items():
        assert freq > 0
        return char

# run with cat 1781c.txt | python3 1781c.py
def generate_candidate_word(word: str, chars2modify: list[str], chars2add: list[str], freq: int) -> str:
    # Greedily chenerate the new word
    cand_word = [None] * len(word)
    # Keep the characters we can
    counts = {char : freq for char in chars2modify}
    # print(counts)
    # print("FREQ IS {}".format(freq))
    for i in range(len(word)):
        char = word[i]
        if char in counts and counts[char] > 0:
            cand_word[i] = word[i]
            counts[char] -= 1
    
    # # XXX
    # print("candidate before modification")
    # print("".join(map(lambda x: "_" if x is None else x, cand_word)))
    # # XXX
    
    # Now replace other characters willy nilly
    counts = {char : counts[char] for char in chars2modify if counts[char] > 0}
    for char in chars2add:
        counts[char] = freq
    
    # print(counts)
    # print(len(counts))
    # print(cand_word.count(None))
    
    for i in range(len(word)):
        if cand_word[i] is None:
            char = rand_char(counts)
            cand_word[i] = char

            counts[char] -= 1
            if counts[char] == 0:
                del counts[char]
    
    assert len(counts) == 0

    # Sanity test
    assert all([cand_word[i] is not None for i in range(len(word))])

    # # XXX
    # print("candidate after modification")
    # print("".join(map(lambda x: "_" if x is None else x, cand_word)))
    # # XXX

    return "".join(cand_word)

def opt(word: str) -> tuple[str, int]:
    # valid number of apperances
    # print(type(math))
    # print(type(math.floor))
    # print(type(math.sqrt))
    # print(type(len(word)))
    max_factor = math.floor(math.sqrt(len(word)))
    # if the max factor divides the length then we'll include it, else
    # we'll include the thing right below the sqrt
    appearances = [l for l in range(1, max_factor + 1) if len(word) % l == 0]
    appearances += [len(word) // l for l in appearances]
    appearances = list(set(appearances))

    best_word = word[0] * len(word)
    best_cost = len(word) - len([i for i in range(len(word)) if word[i] == word[0]])

    # Make sure we know the character frequencies
    orig_char_freqs = {}
    for char in word:
        orig_char_freqs[char] = orig_char_freqs.get(char, 0) + 1
    
    # Greedily pick characters that are closest in frequency to the number of characters
    # (this will take O(sqrt(n)*O(nlogn)))
    for freq in appearances:
        num_chars = len(word) // freq

        # avoid impossible combinations for long strings=> runtime error
        if num_chars <= len("abcdefghijklmnopqrstuvwxyz"):
            closest_char_freqs = sorted(orig_char_freqs.items(), key=lambda x: abs(x[1] - num_chars))[:num_chars]
            # print(closest_char_freqs)

            # If we need to add new chars, add them...
            taken_chars_in_new = len(closest_char_freqs) * freq
            remaining_chars_needed = (len(word) - taken_chars_in_new) // freq
            assert remaining_chars_needed == 0 or len(orig_char_freqs) * freq < len(word)

            chars2modify = [c for c, _ in closest_char_freqs]
            chars2add = list(set("abcdefghijklmnopqrstuvwxyz") - set([c for c, _ in orig_char_freqs.items()]))[:remaining_chars_needed]

            # Greedily generate the new word
            # # XXX
            # print("modify")
            # print(chars2modify)
            # print("add")
            # print(chars2add)
            # print("...")
            # # XXX
            cand_word = generate_candidate_word(word, chars2modify, chars2add, freq)
            assert len(cand_word) == len(word)

            cand_cost = len([i for i in range(len(word)) if word[i] != cand_word[i]])
            if cand_cost < best_cost:
                best_cost = cand_cost
                best_word = cand_word

    return best_word, best_cost

DEBUG = False
if __name__ == "__main__":
    if DEBUG:
        import random

        MAX_STR_LENS = [20, 100, int(10**5)]
        NUMS_TESTS = [100, 100, 10]
        MAX_WORD_PRINT_LEN = 20
        for NUM_TESTS, MAX_STR_LEN in zip(NUMS_TESTS, MAX_STR_LENS):
            for _ in range(NUM_TESTS):
                print("*"*100)
                length = random.randint(1, MAX_STR_LEN)
                randchar = lambda: chr(random.randint(ord('a'), ord('z')))
                word = "".join([randchar() for _ in range(length)])
                
                print("Trying random word of len {}".format(length))
                modded, cost = opt(word)
                wordstr = word if len(word) < MAX_WORD_PRINT_LEN else word[:MAX_WORD_PRINT_LEN] + "..."
                moddedstr = modded if len(modded) < MAX_WORD_PRINT_LEN else modded[:MAX_WORD_PRINT_LEN] + "..."
                print("{} -> {} Have cost {}".format(wordstr, moddedstr, cost))
    else:
        # cli
        T = int(input())
        for _ in range(T):
            # For each test case get the first line as n and m if necessary (we don't have to
            # because our language gets the lengths fortunately)
            # and then get the lines for a and b
            #
            _ = input()
            word = input().strip().decode('utf-8')
            # print(word)
            modded, cost = opt(word)
            print(cost)
            print(modded)
