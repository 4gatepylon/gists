#!/usr/bin/env python3
"""Generate examples and run main.py or an existing executable. No reference solver."""

import argparse
from dataclasses import dataclass
from itertools import permutations
from pathlib import Path
import random
import shlex
import subprocess
import sys
from typing import Optional


@dataclass
class Example:
    name: str
    stdin: str
    answer_count: int
    expected: Optional[str] = None


def single_case(name, skills, removal_order):
    stdin = "\n".join([
        "1",
        str(len(skills)),
        " ".join(map(str, skills)),
        " ".join(map(str, removal_order)),
    ]) + "\n"
    return Example(name, stdin, len(skills))


def make_examples(seed):
    rng = random.Random(seed)
    examples = [Example(
        "published sample (three test cases in one invocation)",
        "3\n4\n1 2 4 3\n1 2 3 4\n"
        "5\n3 1 7 2 6\n3 1 5 2 4\n"
        "6\n10 1 2 20 3 4\n4 1 2 3 5 6\n",
        15,
        "2 1 0 0\n1 0 2 1 0\n1 0 3 2 1 0\n",
    )]

    chosen = [
        ("single minimum", [1], [1]),
        ("single maximum", [10**9], [1]),
        ("equal pair", [5, 5], [1, 2]),
        ("increasing pair", [1, 2], [2, 1]),
        ("decreasing pair", [2, 1], [1, 2]),
        ("descending", [5, 4, 3, 2, 1], [3, 1, 5, 2, 4]),
        ("prefix sum equals jump", [2, 3, 4, 9, 10], [4, 1, 5, 3, 2]),
        ("maximum skills", [10**9] * 5, [5, 3, 1, 4, 2]),
        ("alternating extremes", [1, 10**9, 1, 10**9, 1], [2, 4, 1, 5, 3]),
    ]
    for name, skills, order in chosen:
        examples.append(single_case(name, skills, order))

    # Sample one length-five sequence from each of ten families.
    scale = rng.randint(1, 4)
    start = rng.randint(2, 5)
    step = rng.randint(1, 3)
    prefix = [start + step * i for i in range(3)]
    prefix_sum = sum(prefix)
    irregular = [rng.randint(1, 5)]
    for _ in range(4):
        irregular.append(irregular[-1] + rng.randint(1, 20))

    prototypes = [
        ("powers of two", [scale * 2**i for i in range(5)]),
        ("linear", [start + step * i for i in range(5)]),
        ("quadratic", [scale * i**2 for i in range(1, 6)]),
        ("constant", [rng.randint(1, 20)] * 5),
        ("linear with plateaus", [start + step * i for i in [0, 1, 1, 2, 2]]),
        ("prefix sum one ABOVE jump", prefix + [prefix_sum - 1, prefix_sum + step]),
        ("prefix sum one BELOW jump", prefix + [prefix_sum + 1, prefix_sum + 1 + step]),
        ("cubic", [scale * i**3 for i in range(1, 6)]),
        ("Fibonacci multiples", [scale * i for i in [1, 1, 2, 3, 5]]),
        ("random positive gaps", irregular),
    ]
    possible_orders = list(permutations(range(1, 6)))
    for name, skills in prototypes:
        examples.append(single_case(f"{name} / original order", skills, list(range(1, 6))))
        # Shuffle a independently for each of five distinct removal permutations p.
        for number, order in enumerate(rng.sample(possible_orders, 5), start=1):
            shuffled_skills = rng.sample(skills, len(skills))
            examples.append(single_case(
                f"{name} / shuffled trial {number}", shuffled_skills, order,
            ))

    return examples


def show_input(number, example):
    print(f"\n=== {number}: {example.name} ===")
    print("stdin:")
    print(example.stdin, end="", flush=True)


def show_output(label, value):
    # TimeoutExpired can contain bytes even when subprocess.run uses text=True.
    if isinstance(value, bytes):
        value = value.decode(errors="replace")
    print(f"{label}:")
    print((value or "").rstrip() or "(empty)")


def run_example(command, example, timeout):
    if example.expected is not None:
        show_output("published expected output", example.expected)
    try:
        result = subprocess.run(
            command, input=example.stdin, text=True,
            capture_output=True, timeout=timeout,
        )
    except subprocess.TimeoutExpired as error:
        show_output("program stdout before timeout", error.stdout)
        if error.stderr:
            show_output("program stderr before timeout", error.stderr)
        print(f"FAIL: exceeded {timeout:g} seconds")
        return False

    show_output("program stdout", result.stdout)
    if result.stderr:
        show_output("program stderr", result.stderr)
    if result.returncode != 0:
        print(f"FAIL: exit code {result.returncode}")
        return False
    try:
        answers = [int(token) for token in result.stdout.split()]
    except ValueError:
        print("FAIL: stdout must contain only integers")
        return False
    if len(answers) != example.answer_count:
        print(f"FAIL: expected {example.answer_count} integers; got {len(answers)}")
        return False
    if example.expected is not None:
        if answers != [int(token) for token in example.expected.split()]:
            print("FAIL: differs from the published expected output")
            return False
        print("Matches the published expected output.")
    else:
        print("Output format OK; correctness not checked (no expected output).")
    return True


def run_all(command, examples, timeout):
    failures = 0
    for number, example in enumerate(examples, start=1):
        show_input(number, example)
        if not run_example(command, example, timeout):
            failures += 1
    print(f"\nCompleted {len(examples)} runs: {failures} execution/output-check failures.")
    print("Only the published sample has expected outputs; inspect the other outputs yourself.")
    return int(failures > 0)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--seed", type=int, default=2026, help="random seed (default: 2026)")
    parser.add_argument("--exe", type=Path, help="run an existing executable instead of main.py")
    parser.add_argument("--timeout", type=float, default=2.0, help="seconds per run (default: 2)")
    parser.add_argument("--list", action="store_true", help="print inputs without running the program")
    args = parser.parse_args()
    if not 0 < args.timeout < float("inf"):
        parser.error("--timeout must be a positive, finite number")

    examples = make_examples(args.seed)
    case_count = sum(int(example.stdin.splitlines()[0]) for example in examples)
    print(f"Seed: {args.seed}; {len(examples)} runs ({case_count} test cases).", flush=True)
    if args.list:
        for number, example in enumerate(examples, start=1):
            show_input(number, example)
        return 0

    try:
        if args.exe:
            command = [str(args.exe.expanduser().resolve())]
        else:
            source = Path(__file__).resolve().with_name("main.py")
            command = [sys.executable, str(source)]
        print(f"Run: {shlex.join(command)}", file=sys.stderr, flush=True)
        return run_all(command, examples, args.timeout)
    except OSError as error:
        print(f"error: {error}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
