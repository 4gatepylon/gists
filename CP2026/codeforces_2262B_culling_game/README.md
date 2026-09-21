# 2262B — Culling Game

- **Source:** [Codeforces 2262B](https://codeforces.com/problemset/problem/2262/B)
- **Contest:** Codeforces Round 1120 (Div. 1), problem B
- **Time limit:** 2 seconds per test
- **Memory limit:** 256 megabytes

Adriano did not finish this and might come back to it later.

## Local practice

This problem uses Python, overriding the workspace's Rust default. Fill in
`solve` in `main.py`. The scaffold handles stdin/stdout and keeps removal indices
1-based. It raises `NotImplementedError` until you implement the algorithm.

From this folder, with Python 3.9+:

```sh
python3 run_examples.py
python3 run_examples.py --seed 42
```

The standard-library-only runner invokes `main.py` in a separate process using
the same Python interpreter as the runner, and logs the command. No compilation
is needed. It works from other working directories too; pass the path to
`run_examples.py`. To run your program directly on your own input file:

```sh
python3 main.py < input.txt
```

The default suite has 70 invocations / 72 test cases:

- The published three-case sample, including an expected-output comparison.
- Nine chosen cases: singletons, equal/increasing/decreasing pairs, descending
  skills, an exact prefix-sum/jump boundary, maximum skills, and alternating extremes.
- Ten length-five sequence families, each with one unshuffled `a` and removal
  order `1 2 3 4 5`, plus five trials that independently shuffle the skills `a` and
  removal order `p`: powers of two, linear, quadratic, constant, linear with
  plateaus, a jump with its preceding sum one above or below it, cubic, Fibonacci
  multiples, and random positive gaps. These patterns describe the prototypes
  before shuffling; shuffled inputs need not be increasing or retain the
  prefix/jump boundary.
  The five randomized trials use distinct removal orders; repeated skill values
  can make skill shuffles identical (always so for constants). The seed controls sequence
  parameters and both shuffles. Published samples and chosen cases stay fixed.

Each invocation prints its exact input, the program's stdout, and any stderr or
failure. The runner continues after crashes and timeouts. Only the published
sample has known expected outputs; generated cases check execution and output
format, **not correctness**. The runner contains no solving logic.

Other options:

```sh
python3 run_examples.py --list                 # inspect inputs without running main.py
python3 run_examples.py --exe /path/to/program # run an existing executable
python3 run_examples.py --timeout 5            # seconds per invocation
```

Exit status is `0` when execution/output checks pass, `1` for failed checks or
timeouts, and `2` for setup errors. The default seed is `2026`, so runs reproduce.

## Statement

Bessie is making a show called Moojutsu Cowsen. For one episode, she invites `n`
sorcerers and lines them up from left to right. The initial skill level of the
`i`-th sorcerer is `a_i`.

The sorcerers compete in a king-of-the-hill tournament. The leftmost remaining
sorcerer starts as the champion, and his current skill is equal to his initial
skill.

Then, the champion faces each remaining sorcerer to his right, one by one. Suppose
the current champion has skill `s`, and the next sorcerer has skill `x`.

- If `s < x`, the champion forfeits. The next sorcerer becomes the new champion
  with skill `x`.
- Otherwise, the champion fights and wins (the champion still wins when `s = x`).
  In this case, the champion's current skill becomes `s + x`.

Bessie finds forfeits boring, so before running the tournament, she may remove
some sorcerers from the lineup.

You are given a permutation `p_1, p_2, …, p_n` of the integers from `1` to `n`.
For each `0 ≤ i ≤ n − 1`, Bessie removes sorcerers `p_1, p_2, …, p_i` from the
lineup. If `i = 0`, no sorcerers are removed. The relative order of all remaining
sorcerers does not change.

For each such `i`, determine how many forfeits happen when Bessie runs the
tournament using only the remaining sorcerers.

## Input

Each test contains multiple test cases. The first line contains the number of
test cases `t` (`1 ≤ t ≤ 10^4`). The description of the test cases follows.

For each test case:

- The first line contains a single integer `n` (`1 ≤ n ≤ 2 · 10^5`).
- The second line contains `n` integers `a_1, a_2, …, a_n`
  (`1 ≤ a_i ≤ 10^9`).
- The third line contains a permutation `p_1, p_2, …, p_n` of the integers from
  `1` to `n`.

The sum of `n` over all test cases does not exceed `2 · 10^5`.

## Output

For each test case, output `n` integers.

The `i`-th integer (using 1-based indexing) should be the number of forfeits after
removing sorcerers `p_1, p_2, …, p_(i−1)`. Thus, the first integer corresponds to
no removals, and the last corresponds to one remaining sorcerer.

## Example

### Input

```text
3
4
1 2 4 3
1 2 3 4
5
3 1 7 2 6
3 1 5 2 4
6
10 1 2 20 3 4
4 1 2 3 5 6
```

### Output

```text
2 1 0 0
1 0 2 1 0
1 0 3 2 1 0
```

## Example explanation

For the first test case, the answers are `2, 1, 0, 0`.

- Before any removals, the array is `[1, 2, 4, 3]`. The champion with skill `1`
  forfeits against skill `2`, and then the champion with skill `2` forfeits against
  skill `4`. The champion with skill `4` defeats skill `3`, so there are `2` forfeits.
- After removing sorcerer `1`, the remaining array is `[2, 4, 3]`. The champion
  with skill `2` forfeits against skill `4`, and then the champion with skill `4`
  defeats skill `3`, so there is `1` forfeit.
- After removing sorcerers `1` and `2`, the remaining array is `[4, 3]`. The
  champion defeats the only remaining sorcerer, so there are `0` forfeits. After
  removing sorcerers `1`, `2`, and `3`, only one sorcerer remains, so there are also
  `0` forfeits.

For the second test case, the answers are `1, 0, 2, 1, 0`.

- Before any removals, the array is `[3, 1, 7, 2, 6]`. The champion with skill `3`
  defeats skill `1` and gains one skill point, then forfeits against skill `7`.
  After that, the champion defeats skills `2` and `6`, so there is `1` forfeit.
- After removing sorcerer `3`, whose skill is `7`, the remaining array is
  `[3, 1, 2, 6]`. The champion defeats every remaining sorcerer, so there are
  `0` forfeits.
- After also removing sorcerer `1`, the remaining array is `[1, 2, 6]`. The
  champion with skill `1` forfeits against skill `2`, and then the champion with
  skill `2` forfeits against skill `6`, so there are `2` forfeits.
- After also removing sorcerer `5`, the remaining array is `[1, 2]`. There is
  `1` forfeit. Finally, after also removing sorcerer `2`, only one sorcerer remains,
  so there are `0` forfeits.
