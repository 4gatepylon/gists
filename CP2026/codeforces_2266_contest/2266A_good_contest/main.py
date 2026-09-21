# from __future__ import annotations
import sys
from typing import Tuple    


def solve(n: int, solved: Tuple[int, int, int]) -> int:
    """Return the minimum weak participants; solved contains a_1, a_2, a_3."""
    num_strong = min(solved)
    return n - min(solved) # n is total number of players


def main() -> None:
    tokens = iter(map(int, sys.stdin.buffer.read().split()))
    test_cases = next(tokens)
    output = []
    for _ in range(test_cases):
        n = next(tokens)
        solved = (next(tokens), next(tokens), next(tokens))
        output.append(str(solve(n, solved)))
    sys.stdout.write("\n".join(output) + "\n")


if __name__ == "__main__":
    main()
