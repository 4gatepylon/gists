import sys
from typing import List, Tuple


def solve(a: List[int], b: List[int], edges: List[Tuple[int, int]]) -> int:
    """Return the maximum total value; edges use 1-based nodes, rooted at node 1."""
    raise NotImplementedError("Implement Modular Tree for one test case")


def main() -> None:
    tokens = iter(map(int, sys.stdin.buffer.read().split()))
    test_cases = next(tokens)
    output = []
    for _ in range(test_cases):
        n = next(tokens)
        a = [next(tokens) for _ in range(n)]
        b = [next(tokens) for _ in range(n)]
        edges = [(next(tokens), next(tokens)) for _ in range(n - 1)]
        output.append(str(solve(a, b, edges)))
    sys.stdout.write("\n".join(output) + "\n")


if __name__ == "__main__":
    main()
