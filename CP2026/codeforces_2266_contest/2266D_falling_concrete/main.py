import sys
from typing import List


def solve(heights: List[int]) -> int:
    """Return the maximum achievable flat segment length for one road."""
    raise NotImplementedError("Implement Falling Concrete for one test case")


def main() -> None:
    tokens = iter(map(int, sys.stdin.buffer.read().split()))
    test_cases = next(tokens)
    output = []
    for _ in range(test_cases):
        n = next(tokens)
        heights = [next(tokens) for _ in range(n)]
        output.append(str(solve(heights)))
    sys.stdout.write("\n".join(output) + "\n")


if __name__ == "__main__":
    main()
