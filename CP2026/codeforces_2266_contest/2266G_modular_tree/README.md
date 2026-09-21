# 2266G — Modular Tree

- **Source:** [Codeforces 2266G](https://codeforces.com/contest/2266/problem/G)
- **Time limit:** 3 seconds
- **Memory limit:** 256 MB

## Local practice

Implement `solve(a: List[int], b: List[int], edges: List[Tuple[int, int]]) -> int`
in `main.py` for one test case. The initial stub raises `NotImplementedError`.
Input parsing, test-case iteration, and printing are provided. `List` and `Tuple`
are imported from `typing` for compatibility with older Python runners.

`n` is `len(a)`. The arrays are ordinary Python lists: node `i` has initial
value `a[i - 1]` and modulus `b[i - 1]`. Edge endpoints retain the input's
1-based node numbers. The root is node 1; `edges` is empty when `n = 1`.

From the parent contest folder:

```sh
python3 -m pytest -q 2266G_modular_tree
```

The test runs `main.py` with `sample.in` as stdin and compares stdout with
`sample.expected`. These files are the source of truth for the test.

## Statement

Vihaan has a tree with `n` nodes, rooted at node 1. A tree is an undirected,
connected graph with no cycles.

Each node `i` has an initial value `a_i` and a modulus `b_i`. Its current value
is `x_i`, initially equal to `a_i`.

Vihaan may perform the following operation any number of times:

- Choose a node `u`.
- Let `s` be the sum of the current values of all direct children of `u`.
- Replace `x_u` with `(x_u + s) mod b_u`.

Determine the maximum possible sum of the values of all nodes after any
number of operations.

## Input

- The first line contains `t` (`1 <= t <= 10^4`), the number of test cases.
- Each test case begins with `n` (`1 <= n <= 2 * 10^5`), the number of nodes.
- The second line contains `n` initial values `a_1, ..., a_n`
  (`0 <= a_i < b_i`).
- The third line contains `n` moduli `b_1, ..., b_n`
  (`1 <= b_i <= 10^9`).
- Each of the next `n - 1` lines contains two integers `u, v`
  (`1 <= u, v <= n`), describing an undirected edge.
- The edges are guaranteed to form a tree.
- The sum of `n` across all test cases is at most `2 * 10^5`.

## Output

Print one integer per test case: the maximum possible sum of the values of
all nodes after performing any number of operations.

## Example

### Input

```text
8
1
3
7
2
0 3
5 4
1 2
3
0 2 3
7 3 4
1 2
2 3
3
0 0 1
5 2 2
1 2
2 3
4
1 2 3 4
10 3 4 5
1 2
1 3
1 4
3
0 1 3
10 2 4
1 2
1 3
5
0 0 1 2 3
12 6 9 3 4
1 2
1 3
2 4
3 5
4
0 999999999 999999999 999999999
1000000000 1000000000 1000000000 1000000000
1 2
1 3
1 4
```

### Output

```text
3
7
11
6
18
12
27
3999999996
```

### Note

In the first test case, node 1 has no children, so an operation cannot change
its value. The maximum possible sum is 3.

In the third test case, performing the operation on node 1 three times gives:

```text
[0, 2, 3] -> [2, 2, 3] -> [4, 2, 3] -> [6, 2, 3]
```

The resulting sum is `6 + 2 + 3 = 11`. No sequence of operations can obtain
a larger sum.

In the fourth test case, first perform the operation on node 2:

```text
[0, 0, 1] -> [0, 1, 1]
```

Then perform the operation on node 1 four times:

```text
[0, 1, 1] -> [1, 1, 1] -> [2, 1, 1] -> [3, 1, 1] -> [4, 1, 1]
```

The resulting sum is `4 + 1 + 1 = 6`. No sequence of operations can obtain
a larger sum.
