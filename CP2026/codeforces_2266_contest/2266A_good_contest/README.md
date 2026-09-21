# 2266A — Good Contest

- **Source:** [Codeforces 2266A](https://codeforces.com/contest/2266/problem/A)
- **Time limit:** 1 second
- **Memory limit:** 256 MB

## Local practice

Implement `solve(n: int, solved: Tuple[int, int, int]) -> int` in `main.py`.
It receives one test case; the initial stub raises `NotImplementedError`.
The surrounding code reads all test cases and prints one answer per line.
`Tuple` is imported from `typing` so the annotation also works on Python 3.6.
Do not add `from __future__ import annotations` when targeting Python 3.6;
that feature requires Python 3.7 or newer.

From the parent contest folder, run the supplied examples with pytest:

```sh
python3 -m pytest -q 2266A_good_contest
```

The test runs `main.py` with `sample.in` as stdin and compares stdout with
`sample.expected`. These files are the source of truth for the test.
It is expected to fail until you implement `solve`.
All programs in this contest use Python.

## Statement

The next programming contest has three problems and `n` participants.
Problem 1 is easy, problem 2 is medium, and problem 3 is hard.

A participant is called **weak** if they did not solve all three problems.

Unfortunately, the scoreboard was lost. The only remaining information is an
array `a` of length 3, where `a_i` is the number of participants who solved
problem `i`.

Among all scoreboards consistent with this information, find the minimum
possible number of weak participants.

## Input

The first line contains an integer `t` (`1 <= t <= 3000`), the number of test cases.

Each test case contains:

- One integer `n` (`1 <= n <= 9`), the number of participants.
- Three integers `a_1, a_2, a_3` (`0 <= a_i <= n`), the number of participants
  who solved each problem.

## Output

For each test case, print one integer: the minimum possible number of weak
participants.

## Example

### Input

```text
6
3
3 3 3
4
4 4 3
1
1 1 1
9
9 8 9
5
0 5 5
6
4 3 2
```

### Output

```text
0
1
0
1
5
4
```

### Note

In the first test case, all three participants can have solved all three
problems, so the answer is 0.

In the second test case, participant 1 could have solved only problems 1 and 2,
while participants 2, 3, and 4 solved all three problems. Participant 1 is the
only weak participant, giving the minimum of 1.
