import sys


def solve(skills: list[int], removal_order: list[int]) -> list[int]:
    """Return n answers: answer[i] is after the first i removals.

    n = len(skills). Values in removal_order are 1-based sorcerer indices.
    """
    raise NotImplementedError("Fill in the Culling Game algorithm here")


def main() -> None:
    tokens = iter(map(int, sys.stdin.buffer.read().split()))
    test_cases = next(tokens)
    output = []

    for _ in range(test_cases):
        n = next(tokens)
        skills = [next(tokens) for _ in range(n)]
        removal_order = [next(tokens) for _ in range(n)]

        answers = solve(skills, removal_order)
        if len(answers) != n:
            raise ValueError("solve must return n answers")
        output.append(" ".join(map(str, answers)))

    sys.stdout.write("\n".join(output) + "\n")


if __name__ == "__main__":
    main()
