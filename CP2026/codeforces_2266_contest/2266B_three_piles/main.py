import sys


def solve(a: int, b: int, c: int) -> int:
    """Return the final score for one game under optimal play."""
    if a >= b:
        return a + c - b
    counterfactual = abs(a + c - b)
    if b - a >= counterfactual:
        return b - a
    return counterfactual


def main() -> None:
    tokens = iter(map(int, sys.stdin.buffer.read().split()))
    test_cases = next(tokens)
    output = []
    for _ in range(test_cases):
        a, b, c = next(tokens), next(tokens), next(tokens)
        output.append(str(solve(a, b, c)))
    sys.stdout.write("\n".join(output) + "\n")


if __name__ == "__main__":
    main()
