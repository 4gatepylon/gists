# 2266C — AND, OR, Sort!

- **Source:** [Codeforces 2266C](https://codeforces.com/contest/2266/problem/C)
- **Time limit:** 2 seconds
- **Memory limit:** 256 MB

## Local practice

Implement `solve(s: str) -> int` in `main.py` for one test case.
The initial stub raises `NotImplementedError`. Input parsing, test-case iteration,
and printing are provided. `n` is `len(s)`; leading zeros are preserved.

From the parent contest folder:

```sh
python3 -m pytest -q 2266C_and_or_sort
```

The test runs `main.py` with `sample.in` as stdin and compares stdout with
`sample.expected`. These files are the source of truth for the test.

## Statement

You are given a binary string `s` of length `n`, containing only `0` and `1`.

You may perform the following operation any number of times, possibly zero:
choose `i` (`1 <= i <= n`) and replace `s_i` with either the bitwise AND or the
bitwise OR of `s_1, s_2, ..., s_i`.

The bitwise AND or OR of a single element equals that element itself.

Find the minimum number of operations needed to make the string non-decreasing:
`s_1 <= s_2 <= ... <= s_n`.

## Input

- The first line contains `t` (`1 <= t <= 10^4`), the number of test cases.
- Each test case begins with `n` (`2 <= n <= 2 * 10^5`).
- The next line contains the binary string `s` of length `n`.
- The sum of `n` across all test cases is at most `2 * 10^5`.

## Output

Print one integer per test case: the minimum number of operations required to
sort the string in non-decreasing order.

## Example

### Input

```text
6
4
0011
4
1000
5
01000
8
01001101
7
0101010
7
0111101
```

### Output

```text
0
3
1
2
3
1
```

### Note

In the first test case, the string is already sorted, so no operations are
needed. In the second, bitwise OR can change the last three characters to `1`,
producing `1111` in three operations.
