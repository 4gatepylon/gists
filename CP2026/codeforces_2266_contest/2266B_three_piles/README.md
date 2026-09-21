# 2266B — Three Piles

- **Source:** [Codeforces 2266B](https://codeforces.com/contest/2266/problem/B)
- **Time limit:** 1 second
- **Memory limit:** 256 MB

## Local practice

Implement `solve(a: int, b: int, c: int) -> int` in `main.py` for one test case.
The initial stub raises `NotImplementedError`. Input parsing, test-case iteration,
and printing are provided.

From the parent contest folder:

```sh
python3 -m pytest -q 2266B_three_piles
```

The test runs `main.py` with `sample.in` as stdin and compares stdout with
`sample.expected`. These files are the source of truth for the test.

## Statement

Alice and Bob play with three piles of stones. Initially, Alice has `a`
stones, Bob has `b` stones, and the third pile has `c` stones.

They take turns, with Alice first. On a turn, the current player may take any
number of stones from the third pile, possibly zero, and add them to their own
pile. If both players take zero stones on two consecutive turns, the game ends.

Let `A` and `B` be their final numbers of stones. The score is `|A - B|`.
Alice maximizes the score, while Bob minimizes it. Find the final score when
both play optimally.

## Input

- The first line contains `t` (`1 <= t <= 10^4`), the number of test cases.
- Each test case contains `a`, `b`, and `c` (`0 <= a, b, c <= 10^9`).

## Output

Print one integer per test case: the final score under optimal play.
The statement requires a 64-bit integer type in fixed-width languages; Python
integers support these values.

## Example

### Input

```text
5
3 6 3
3 6 10
5 5 4
2 5 6
67676767 41414141 998244353
```

### Output

```text
3
7
4
3
1024506979
```

### Note

In the first test case, Alice can take zero stones, then Bob can take zero
too. The piles remain 3 and 6, giving a score of 3. Alice cannot achieve a larger
score, and Bob cannot achieve a smaller one.

In the second test case, Alice can take all 10 stones on her first turn. The
game then ends with piles of sizes 13 and 6, giving a score of 7.
