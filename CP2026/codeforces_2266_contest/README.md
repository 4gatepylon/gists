# Codeforces contest 2266

[Contest page](https://codeforces.com/contest/2266)

**All programs in this contest use Python**, overriding CP2026's Rust default.
Each problem folder contains `main.py`, its statement, `sample.in`,
`sample.expected`, and pytest tests. Implement only the typed `solve` function
for one test case; input parsing, test-case iteration, and printing are provided.
Each solving function starts as a stub raising `NotImplementedError`.

The submission scaffolds use container annotations from `typing` (`List`,
`Tuple`, etc.) for Python 3.6 compatibility. Do not add
`from __future__ import annotations`. Follow the Codeforces compatibility
section in the repository's [AGENTS.md](../../AGENTS.md).

| Problem | Folder |
| --- | --- |
| [A — Good Contest](https://codeforces.com/contest/2266/problem/A) | [2266A_good_contest](2266A_good_contest/) |
| [B — Three Piles](https://codeforces.com/contest/2266/problem/B) | [2266B_three_piles](2266B_three_piles/) |
| [C — AND, OR, Sort!](https://codeforces.com/contest/2266/problem/C) | [2266C_and_or_sort](2266C_and_or_sort/) |
| [D — Falling Concrete](https://codeforces.com/contest/2266/problem/D) | [2266D_falling_concrete](2266D_falling_concrete/) |
| [E — Prime Destruction](https://codeforces.com/contest/2266/problem/E) | [2266E_prime_destruction](2266E_prime_destruction/) |
| [F — MEX Replacement](https://codeforces.com/contest/2266/problem/F) | [2266F_mex_replacement](2266F_mex_replacement/) |
| [G — Modular Tree](https://codeforces.com/contest/2266/problem/G) | [2266G_modular_tree](2266G_modular_tree/) |
| [H — Deque Malfunction](https://codeforces.com/contest/2266/problem/H) | [2266H_deque_malfunction](2266H_deque_malfunction/) |

**Practice status:** D — Falling Concrete is not implemented, and F–H were not
attempted. Their folders contain the statements, boilerplate, and samples in
case we return to these problems later.

## Run the examples

Use a local Python 3.9+ interpreter with a compatible pytest release. From this
contest folder:

```sh
python3 -m pytest -q
```

To test one problem, pass its folder, for example:

```sh
python3 -m pytest -q 2266A_good_contest
python3 -m pytest -q 2266B_three_piles
python3 -m pytest -q 2266C_and_or_sort
python3 -m pytest -q 2266G_modular_tree
python3 -m pytest -q 2266H_deque_malfunction
```

If needed, install the test dependency with
`python3 -m pip install -r requirements-dev.txt`.

Each problem has one test: run `main.py` with `sample.in` as stdin, check that
the process succeeds, and compare stdout with `sample.expected`, ignoring
whitespace differences. These two files are the source of truth; the tests
contain no sample values or input parsing logic. To add or edit cases, update
both files, including the test-case count at the start of `sample.in`.
Tests are expected to fail until you implement each function.

To run a program directly, for example:

```sh
python3 2266A_good_contest/main.py < 2266A_good_contest/sample.in
```
