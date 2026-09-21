# 2266F — MEX Replacement

- **Source:** [Codeforces 2266F](https://codeforces.com/contest/2266/problem/F)
- **Time limit:** 2 seconds
- **Memory limit:** 256 MB

## Local practice

Implement `solve(entries: List[Tuple[int, int]]) -> int` in `main.py` for one test case.
The initial stub raises `NotImplementedError`. Input parsing, test-case iteration,
and printing are provided. `entries` contains the compressed `(value, count)`
pairs; `n` is `len(entries)`.

From the parent contest folder:

```sh
python3 -m pytest -q 2266F_mex_replacement
```

The test runs `main.py` with `sample.in` as stdin and compares stdout with
`sample.expected`. These files are the source of truth for the test.

## Statement

You are given a multiset of non-negative integers in compressed form:
each pair `(x_i, y_i)` says that value `x_i` occurs `y_i` times.

In one operation, choose a nonempty collection of elements currently in the
multiset, remove those elements, and insert one element equal to their MEX.
The MEX is the smallest non-negative integer absent from the chosen collection.

You may perform any number of operations, possibly zero. Find the maximum
integer that can appear in the multiset at any moment.

## Input

- The first line contains `t` (`1 <= t <= 10^4`), the number of test cases.
- Each test case begins with `n` (`1 <= n <= 2 * 10^5`), the number of distinct
  initial values.
- The next `n` lines contain pairs `x_i, y_i`
  (`0 <= x_i <= 10^9`, `1 <= y_i <= 10^9`).
- The `x_i` values are pairwise distinct.
- The sum of `n` across all test cases is at most `2 * 10^5`.

## Output

Print one integer per test case: the maximum integer that can ever appear.

## Example

### Input

```text
10
4
2 2
0 3
4 1
1 2
1
10 1
1
2 3
3
0 1
2 2
3 1
3
0 4
1 2
2 1
1
0 100
2
2 7
3 1
2
0 1
3 3
3
0 1
1 1
59 1
4
0 5
1 1
2 3
3 1
```

### Output

```text
5
10
3
4
4
7
4
3
59
5
```

### Note

In the first test case, the initial multiset is `{0, 0, 0, 1, 1, 2, 2, 4}`.
Replacing `{0, 1, 2}` with `3` leaves `{0, 0, 1, 2, 3, 4}`. Replacing
`{0, 1, 2, 3, 4}` with `5` produces the maximum achievable value, 5.

In the second test case, 10 is initially present, and no greater value can
appear, so the answer is 10.
