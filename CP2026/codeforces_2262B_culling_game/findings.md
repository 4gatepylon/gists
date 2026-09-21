# Findings

```text
- One deletion can create forfeits at every remaining comparison:
  [X, X-1, 2(X-1), 4(X-1), ...], X >= 2, all values <= 10^9.
  Each entry after X equals the preceding sum minus 1: initially no forfeits.
  Removing X leaves successive doublings: L-2 forfeits for original length L.
  This concerns one deletion, not total changes across all deletions.

- Naively correcting every trajectory entry in [i,j] can take Theta(n^2):
  use a = [n, 1, ..., 1] with zero-based removal order [1, 2, ..., n-1, 0].
  There are never any forfeits, so each correction scans to the end.
  Updating the remaining suffixes costs (n-2) + (n-3) + ... + 0 = Theta(n^2),
  even though every answer is 0.

- In a fixed lineup, the champion's skill strictly increases at each encounter.
- Deleting to the left cannot increase a surviving non-first position's incoming
  champion skill. After deleting i, the champion/skill trajectory matches again
  at the next pre-deletion forfeit position: only [i,j] can differ, where j is
  just before that position (or the last index if none).
- After one deletion, the change in total forfeits is either -1 or nonnegative:
  the count can increase or stay unchanged, but decrease by at most 1.
- The last answer is 0: one sorcerer remains.
- The last permutation entry is determined by the rest; its removal is never queried.
- Deletion is equivalent to setting a value to 0, provided the initial champion
  is the first nonzero element. Later zeros add no skill and cause no forfeits.
```
