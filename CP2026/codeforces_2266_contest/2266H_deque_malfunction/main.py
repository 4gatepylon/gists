import sys
from typing import List


def solve(n: int, arrivals: List[int]) -> int:
    """Return the minimum malfunctions to obtain [1, ..., n], or -1 if impossible."""
    raise NotImplementedError("Implement Deque Malfunction for one test case")


def main() -> None:
    tokens = iter(map(int, sys.stdin.buffer.read().split()))
    test_cases = next(tokens)
    output = []
    for _ in range(test_cases):
        n, m = next(tokens), next(tokens)
        arrivals = [next(tokens) for _ in range(m)]
        output.append(str(solve(n, arrivals)))
    sys.stdout.write("\n".join(output) + "\n")


if __name__ == "__main__":
    main()
