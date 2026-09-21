import sys
from typing import List, Tuple


def solve(entries: List[Tuple[int, int]]) -> int:
    """Return the maximum achievable value; entries contains (value, count) pairs."""
    raise NotImplementedError("Implement MEX Replacement for one test case")


def main() -> None:
    tokens = iter(map(int, sys.stdin.buffer.read().split()))
    test_cases = next(tokens)
    output = []
    for _ in range(test_cases):
        n = next(tokens)
        entries = [(next(tokens), next(tokens)) for _ in range(n)]
        output.append(str(solve(entries)))
    sys.stdout.write("\n".join(output) + "\n")


if __name__ == "__main__":
    main()
