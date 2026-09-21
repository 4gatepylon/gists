# 2266E — Prime Destruction

- **Source:** [Codeforces 2266E](https://codeforces.com/contest/2266/problem/E)
- **Time limit:** 2 seconds
- **Memory limit:** 256 MB

## Local practice

Implement `solve(values: List[int], k: int) -> int` in `main.py` for one test case.
The initial stub raises `NotImplementedError`. Input parsing, test-case iteration,
and printing are provided. `n` is `len(values)`.

From the parent contest folder:

```sh
python3 -m pytest -q 2266E_prime_destruction
```

The test runs `main.py` with `sample.in` as stdin and compares stdout with
`sample.expected`. These files are the source of truth for the test.

## Statement

You are given a multiset `a` of `n` positive integers.

You may perform the following operation any number of times, possibly zero:
choose an element `x > 1` and a prime divisor `p` of `x`, remove one occurrence
of `x`, and insert `p` copies of `x / p`.

Given `k` (`1 <= k <= n`), let `f(k)` be the minimum number of operations,
starting from the original multiset, required until every element is at most `k`.
Find `f(k)`.

## Input

- The first line contains `t` (`1 <= t <= 10^4`), the number of test cases.
- Each test case begins with `n` and `k` (`1 <= k <= n <= 2 * 10^5`).
- The next line contains `n` integers (`1 <= a_i <= n`).
- The sum of `n` across all test cases is at most `2 * 10^5`.

## Output

Print one integer per test case: `f(k)`.

## Example

### Input

```text
6
1 1
1
6 2
6 6 4 3 2 1
8 1
8 6 4 3 2 1 8 6
12 3
12 10 9 8 7 6 5 4 3 2 1 12
10 9
10 9 8 7 6 5 4 3 2 1
5 5
5 4 3 2 1
```

### Output

```text
0
4
25
15
1
0
```

### Note

In the first test case, the only element is already at most `k`.

In the second test case, the following four operations suffice, and fewer
operations cannot achieve the required result:

```text
[6, 6, 4, 3, 2, 1]
[2, 2, 2, 6, 4, 3, 2, 1]
[2, 2, 2, 2, 2, 2, 4, 3, 2, 1]
[2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 1]
[2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 1]
```
