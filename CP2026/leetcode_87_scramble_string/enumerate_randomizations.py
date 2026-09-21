"""Print your generated randomizations for source prefixes of lengths 1 <= n < N."""

import argparse
import math
from string import ascii_lowercase
from typing import Iterable


# Brutce force :shrug:
def _generate_randomizations(s: str) -> Iterable[str]:
    """Return all strings reachable by the statement's recursive split-and-swap process."""
    assert len(s) > 0
    if len(s) == 1:
        yield s
    else:
        for i in range(1, len(s)):
            left = s[:i]
            right = s[i:]
            for left_rand in _generate_randomizations(left):
                for right_rand in _generate_randomizations(right):
                    yield left_rand + right_rand
                    yield right_rand + left_rand


def generate_randomizations(s: str) -> Iterable[str]:
    return sorted(set(_generate_randomizations(s)))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("N", type=int, help="exclusive upper bound on string length")
    parser.add_argument(
        "--source",
        default=ascii_lowercase,
        help="take prefixes of this string (default: lowercase English alphabet)",
    )
    parser.add_argument(
        "--print-list",
        action="store_true",
        help="print each randomization",
    )
    args = parser.parse_args()
    if args.N < 1:
        parser.error("N must be at least 1")
    if args.N - 1 > len(args.source):
        parser.error("--source must contain at least N - 1 characters")

    for n in range(1, args.N):
        s = args.source[:n]
        print(f"n={n}, source={s!r}", flush=True)
        randomizations = generate_randomizations(s)
        assert 1 <= len(randomizations)
        assert len(randomizations) <= math.factorial(len(s))
        if args.print_list:
            for randomization in randomizations:
                print(f"  {randomization!r}")
            print()
        print(
            f"1 <= Number of randomizations={len(randomizations)} <= {math.factorial(len(s))} (2**{len(s)}={2**len(s)})"
        )
        print("=" * 80, flush=True)


if __name__ == "__main__":
    main()
