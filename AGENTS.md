# Repository guide

This repository contains personal practice, coursework, snippets, and experiments.
The pre-existing practice/project folders were last changed in Git in April–June
2023; their contents may be older than that history. `CP2026/` starts the 2026 work.

| Folder | Purpose |
| --- | --- |
| `CP2026/` | Competitive programming and interview-style problem practice for 2026; follow its `README.md`. |
| `Algos/` | Earlier LeetCode, Codeforces, Google Code Jam, and miscellaneous algorithm practice, including unfinished attempts. |
| `CTFs/` | Capture-the-flag competition/task snippets and experiments. |
| `LearningLanguages/` | Exercises and experiments while learning C++, Rust, and SQL, including algorithm problems. |
| `Scripts/` | Utility scripts; currently a script for inspecting file sizes in Git history. |
| `SnippetsUnfinished/` | Assorted snippets, unfinished projects, and experiments with tools and APIs. |
| `Stanford-CS142/` | Stanford CS142 web development coursework, organized by project. |
| `udemy-webdev/` | HTML/CSS exercises and web development course practice. |
| `.cursor/`, `.vscode/` | Editor configuration. |

## Choosing a folder

- Confirm the working folder with the user when the task does not already specify
  or confirm it. An explicit path or earlier confirmation in the conversation is
  sufficient; do not ask again unnecessarily.
- If the current year is 2026 and the user is doing competitive programming,
  LeetCode, Codeforces, or similar practice, `CP2026/` is most likely the right
  folder. Get user confirmation before acting on that inference.
- When working in `CP2026/`, **read `CP2026/README.md` first** and follow its naming,
  grouping, and file conventions. Read the relevant problem/group README too.

## Educational assistance

These rules apply to competitive programming and algorithm practice throughout
the repository, especially `CP2026/`.

- Let the user attempt the problem. Never reveal the solution, key algorithm,
  solution pseudocode, or write problem-solving code right out of the gate.
- Setting up a problem means preserving its statement, constraints, examples,
  and source link. Do not add unsolicited hints, algorithm tags, editorials, or
  solution files.
- Help with understanding the statement and reviewing the user's own reasoning
  or code. When hints are requested, give incremental help at the requested
  level. Provide a full solution or implementation only when explicitly requested repeatedly.
  Do not immediately splurge a solution under any circumstances. You should always ask something like "are you sure?"
  and the user must say yes at least twice (i.e. to two confirmations).
- Boilerplate help is welcome and encouraged: stdin/stdout handling, parsing the
  stated input, empty function stubs, compilation/run commands, and sample-test
  setup. Keep the actual solving logic for the user unless they ask for it.

## Codeforces Python compatibility

- Use annotations imported from `typing` for Codeforces Python submissions:
  `Tuple[int, int, int]`, `List[int]`, `List[Tuple[int, int]]`, `Dict[int, int]`,
  and `Set[int]`. Import only the names needed. Use `Optional[T]` and
  `Union[T, U]` for optional/union types. Plain `int`, `str`, `bool`, and `None`
  annotations are fine.
- **Do not use `from __future__ import annotations`; use `typing` instead.**
  That future import requires Python 3.7+, so it fails on Python 3.6 runners.
  Built-in generic annotations such as `tuple[int, int]` and `list[int]`
  require Python 3.9+ when evaluated. See
  [Python's compatibility notes](https://peps.python.org/pep-0585/#backwards-compatibility).
- Match syntax and standard-library features to the selected judge interpreter.
  Passing pytest on a newer local Python does not establish judge compatibility.
  When available, also run the sample through the target interpreter; clearly
  distinguish a syntax check from actually running that Python version.

Observed with Codeforces 2266A using the `PyPy 3` selection: a submission failed
at line 1 with:

```text
SyntaxError: future feature annotations is not defined
```

Removing the future import and using `from typing import Tuple` with
`Tuple[int, int, int]` fixed the submission. This was an annotation compatibility
issue, not a stdin/stdout issue.
