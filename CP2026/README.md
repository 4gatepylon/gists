# CP2026

Competitive programming and interview-style problem practice for 2026.
Each immediate subfolder is a logical group named `<source>_<name>/`: one problem,
one contest, or one practice session (problems attempted in a single sitting).

## Naming

Use lowercase source names (`codeforces`, `leetcode`, etc.) and lowercase titles
with underscores instead of spaces/punctuation. Preserve official problem IDs,
including Codeforces' uppercase problem letters and any suffix digits.

| Group | Format / example |
| --- | --- |
| Codeforces problem | `codeforces_<contest-id><index>_<title>/`, e.g. `codeforces_2262B_culling_game/`. Keep indices such as `B1` or `B2` intact. |
| LeetCode problem | `leetcode_<problem-number>_<title>/`, e.g. `leetcode_1_two_sum/`. Use the displayed problem number without zero-padding. |
| Codeforces contest | `codeforces_<contest-id>_<contest-name>/`, e.g. `codeforces_2262_round_1120_div1/`. The contest ID is the URL's `2262`, distinct from round number `1120`. |
| LeetCode contest | `leetcode_weekly_<contest-number>/` or `leetcode_biweekly_<contest-number>/`. |
| Practice session | `<source>_practice_<YYYY-MM-DD>[_<label>]/`, e.g. `leetcode_practice_2026-09-19/`. Add a label or session number if needed to distinguish sittings on the same date. |

Use `mixed` as the source for a practice session spanning multiple platforms.
If a platform has no problem identifier, use its title alone after the source.

## Inside each group

- Keep a `README.md` identifying the problem or group, source URL(s), and included
  problem IDs/titles. For a single problem, include the statement, input/output
  constraints, and examples; omit navigation, tags, and editorial material.
- For a single problem, keep its files directly in the group folder. For contests
  or sessions, use one subfolder per problem named `<id>_<title>/`, such as
  `2262B_culling_game/` or `1_two_sum/`, with its own README. For mixed sessions,
  include the source in those names too, e.g. `leetcode_1_two_sum/`.
- Use **Rust by default** (`main.rs`) unless the user asks for another language;
  use descriptive filenames for multiple attempts. Optional sample files can use
  `sample.in` and `sample.expected` (`*.out` is ignored by this repository).

## Boilerplate and local testing

Input/output scaffolding and scripts for examples, testing, or performance testing
are welcome. Write these helper scripts in plain Python, preferably using only
the standard library, and keep them easy to review. A test runner should invoke
the user's program; do not implement a reference solver unless explicitly asked.
Distinguish published expected outputs from generated cases whose outputs are
only being inspected.

For compiled languages, prefer a compile-and-run mode without persistent build
artifacts when available. Otherwise, compile into a temporary directory, log its
path and the compile command, run the executable, and clean up afterward. Rust
scaffolds here can use `rustc --edition=2021 -O main.rs -o <tempdir>/main`; keep
binaries and other build artifacts out of the problem folder.

This is an educational workspace. Follow the repository's `AGENTS.md`: let the
user try solving first, offer boilerplate assistance freely, and avoid unsolicited
solutions or hints.
