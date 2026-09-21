# 87 — Scramble String

**Source:** [LeetCode 87](https://leetcode.com/problems/scramble-string/)

Adriano did not finish this and might come back to it later.

## Problem

To scramble a string:

1. Stop when its length is 1.
2. Otherwise, split it into two nonempty, contiguous substrings.
3. Keep those substrings in order or swap them.
4. Recursively apply the same process to each substring.

Given equal-length strings `s1` and `s2`, return whether this process can turn
`s1` into `s2`.

Constraints: `1 <= len(s1) == len(s2) <= 30`; both strings contain only lowercase
English letters.

| `s1` | `s2` | Result |
| --- | --- | --- |
| `great` | `rgeat` | `true` |
| `abcde` | `caebd` | `false` |
| `a` | `a` | `true` |

## Local enumeration challenge

This preparatory exercise uses a plain-Python script. Implement the typed
`generate_randomizations(s)` function in `enumerate_randomizations.py`.
It currently raises `NotImplementedError`; no generation logic is supplied.
Here, "randomizations" means all outcomes of the recursive split-and-swap process
in the statement.

For each `n` in `range(1, N)`, the wrapper passes the first `n` characters of a
source string to your function, removes duplicate results, and prints them in
lexicographic order under a heading for that length. Return an iterable of strings
(for example, a list or set). You do not need to sort the results yourself.

From this folder, with Python 3.9+:

```sh
python3 enumerate_randomizations.py 6
python3 enumerate_randomizations.py 5 --source aabb
```

The first command uses `a`, `ab`, `abc`, `abcd`, and `abcde`: **N is exclusive**.
The default source is `abcdefghijklmnopqrstuvwxyz`, so it supports `1 <= N <= 27`.
Use `--source` for another sequence, including repeated letters or a longer input;
it must contain at least `N - 1` characters. `N = 1` produces no output.

The wrapper formats your results; it does not check their correctness against a
reference generator. The LeetCode decision problem is left for later.
