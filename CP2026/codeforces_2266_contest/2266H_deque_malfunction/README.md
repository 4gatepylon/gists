# 2266H — Deque Malfunction

- **Source:** [Codeforces 2266H](https://codeforces.com/contest/2266/problem/H)
- **Time limit:** 3 seconds
- **Memory limit:** 256 MB

## Local practice

Implement `solve(n: int, arrivals: List[int]) -> int` in `main.py` for one test
case. The initial stub raises `NotImplementedError`. Input parsing, test-case
iteration, and printing are provided. `List` is imported from `typing` for
compatibility with older Python runners.

`n` is the desired deque length. `arrivals` contains the input array `b` in its
original order; `m` is `len(arrivals)`.

From the parent contest folder:

```sh
python3 -m pytest -q 2266H_deque_malfunction
```

The test runs `main.py` with `sample.in` as stdin and compares stdout with
`sample.expected`. These files are the source of truth for the test.

## Statement

Vihaan built a deque that sometimes malfunctions. He wants its final contents,
from front to back, to be exactly `[1, 2, ..., n]`.

The deque starts empty. You are given an array `b` of length `m`. Its elements
arrive one at a time in the order `b_1, b_2, ..., b_m`.

Whenever `b_i` arrives, Vihaan must perform exactly one of these operations:

- Push `b_i` to the front of the deque.
- Push `b_i` to the back of the deque.
- Make the deque malfunction and insert `b_i` at any position of his choice.

At any time, Vihaan may also remove the element at the front or at the back
of the deque.

After all `m` elements have arrived and all operations have been performed,
the deque must contain exactly `[1, 2, ..., n]` in that order.

Determine the minimum number of malfunctions required, or determine that it
is impossible.

## Input

- The first line contains `t` (`1 <= t <= 10^4`), the number of test cases.
- Each test case begins with `n, m` (`1 <= n <= m <= 2 * 10^5`), the desired
  deque length and the number of arriving elements.
- The second line contains `m` integers `b_1, ..., b_m`
  (`1 <= b_i <= n`) in arrival order.
- The sum of `m` across all test cases is at most `2 * 10^5`.

## Output

Print one integer per test case: the minimum number of malfunctions needed.
If it is impossible to obtain the desired deque, print `-1`.

## Example

### Input

```text
9
1 3
1 1 1
3 4
2 1 1 2
4 4
1 4 2 3
4 5
4 3 4 2 1
5 8
1 3 2 5 4 3 1 2
6 6
1 6 5 4 3 2
4 6
1 3 2 3 4 3
6 8
2 6 3 5 2 1 4 2
6 8
1 6 2 5 1 5 3 4
```

### Output

```text
0
-1
2
0
1
4
0
2
3
```

### Note

In the first test case, push the first 1 to the front, then immediately pop it.
Do the same with the second 1, then push the last 1 to the back:

```text
[] -> [1] -> [] -> [1] -> [] -> [1]
```

No malfunctions are required.

In the second test case, 3 never appears in `b`, so the desired deque is
impossible to obtain.

In the third test case, push 1 and then 4 to the back. Use one malfunction
to insert 2 between them, and another to insert 3 between 2 and 4:

```text
[] -> [1] -> [1, 4] -> [1, 2, 4] -> [1, 2, 3, 4]
```

No sequence using fewer than two malfunctions can obtain the desired deque.

In the fourth test case, push the first 4 to the front and immediately pop it.
Push 3 and the second 4 to the back, then push 2 and 1 to the front:

```text
[] -> [4] -> [] -> [3] -> [3, 4] -> [2, 3, 4] -> [1, 2, 3, 4]
```

No malfunctions are required.
