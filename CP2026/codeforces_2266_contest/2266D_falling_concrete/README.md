# 2266D — Falling Concrete

- **Source:** [Codeforces 2266D](https://codeforces.com/contest/2266/problem/D)
- **Time limit:** 2 seconds
- **Memory limit:** 256 MB

## Local practice

Implement `solve(heights: List[int]) -> int` in `main.py` for one test case.
The initial stub raises `NotImplementedError`. Input parsing, test-case iteration,
and printing are provided. `n` is `len(heights)`.

From the parent contest folder:

```sh
python3 -m pytest -q 2266D_falling_concrete
```

The test runs `main.py` with `sample.in` as stdin and compares stdout with
`sample.expected`. These files are the source of truth for the test.

## Statement

Vihaan is repairing a road with `n` sections. Section `i` has height `a_i`.

In one operation, he chooses `1 <= i < j <= n`, picks up section `j`, and carries
it backwards to position `i`. One unit of concrete falls from it onto each
section it passes over. The carried section is then inserted at position `i`.

Equivalently, the segment
`[a_i, a_(i+1), ..., a_(j-1), a_j]` becomes
`[a_j - (j-i), a_i + 1, a_(i+1) + 1, ..., a_(j-1) + 1]`.

A flat part is a contiguous segment whose sections have equal heights.
After any number of operations, possibly zero, what is the maximum possible
length of a flat part?

## Input

- The first line contains `t` (`1 <= t <= 10^4`), the number of test cases.
- Each test case begins with `n` (`1 <= n <= 2 * 10^5`).
- The next line contains `n` heights (`n <= a_i <= 10^9`).
- The sum of `n` across all test cases is at most `2 * 10^5`.

Under these constraints, every section remains positive after any sequence
of operations.

## Output

Print one integer per test case: the maximum possible length of a flat part.

## Example

### Input

```text
8
5
5 5 5 9 8
6
6 6 6 6 6 6
5
5 6 7 8 9
5
9 7 12 10 12
4
4 7 5 8
7
14 9 14 12 8 11 12
5
1000000000 1000000000 1000000000 1000000000 1000000000
8
8 8 12 8 14 10 15 13
```

### Output

```text
4
6
1
5
4
2
5
6
```

### Note

In the first test case, moving section 4 to position 1 gives
`[5, 5, 5, 9, 8] -> [6, 6, 6, 6, 8]`, creating a flat part of length 4.

In the fourth test case, use moves `3 -> 1`, `4 -> 3`, then `5 -> 3`:

```text
[9, 7, 12, 10, 12]
[10, 10, 8, 10, 12]
[10, 10, 9, 9, 12]
[10, 10, 10, 10, 10]
```

In the fifth test case, use moves `2 -> 1` then `4 -> 2`:
`[4, 7, 5, 8] -> [6, 5, 5, 8] -> [6, 6, 6, 6]`.
