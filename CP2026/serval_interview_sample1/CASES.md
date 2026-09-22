# Motion test cases

Fixtures: [cases.py](cases.py). Contract: [README.md](README.md).

## Case partitions

### Part 1: input and output lengths

43 cases. Axes: **number of input readings × number of output periods**.
`L` = below threshold, `E` = equal, `H` = above.
Cells show representative existing dynamics, not every fixture.

```text
+----------------+-----------+--------------------------+--------------------------+-------------------+
| Input readings | 0 periods | 1 period                 | 2 periods                | >2 periods        |
+----------------+-----------+--------------------------+--------------------------+-------------------+
| 0              | empty     | impossible               | impossible               | impossible        |
+----------------+-----------+--------------------------+--------------------------+-------------------+
| 1              | L         | E, H                     | impossible               | impossible        |
+----------------+-----------+--------------------------+--------------------------+-------------------+
| 2              | MISSING   | MISSING                  | impossible               | impossible        |
+----------------+-----------+--------------------------+--------------------------+-------------------+
| 3              | LLL       | HHH, EEE, HLL, LHL, LLH, | HLH (sparse timestamps), | impossible        |
|                |           | HHL, LHH, HEH            | ELH (times a, a+.5, a+1) |                   |
+----------------+-----------+--------------------------+--------------------------+-------------------+
| >3             | MISSING   | EHEH, LHHL, LEEL,        | HHLLLHH,                 | HLELH,            |
|                |           | equality at run ends     | HELHE (fractional gap),  | mixed run lengths |
|                |           |                          | HHLEH (tiny gap)         |                   |
+----------------+-----------+--------------------------+--------------------------+-------------------+
```

**MISSING** = possible but untested. **Impossible** = no valid input of that
length can produce that many periods. Timing and threshold variants belong
inside the corresponding cells.

### Part 2: number of streams

70 cases, grouped by stream count and primary behavior:

  - Zero streams (1): empty-input behavior.
  - One stream (4)
    - Activity cardinality (2): empty stream or one run.
    - Multiple runs (2): wider gaps or gaps between consecutive integers.
  - Two streams (50)
    - Inactive cameras (2): both inactive or one just below threshold.
    - One-period geometry (7): identical, partial overlap, containment,
      equal starts/ends, and reversed arrangements.
    - Disjoint activity (5): either time ordering, consecutive integer
      endpoints, interleaved runs, distinct singleton times.
    - Closed endpoints and singletons (5): shared endpoint, equal singletons,
      singleton inside/at either end of a period.
    - Overlap multiplicity and gaps (9): one/two/three overlaps on a segment,
      singleton overlaps, small gaps, mixed gap sizes.
    - Multiple runs in both streams (8): repeated fragments, changing end order,
      unmatched heads/tails, tied ends, repeated endpoint overlaps, combined gaps.
    - Sampling and time range (3): distinct sample times, zero/large timestamps.
    - Threshold values (5): equality at boundaries/singletons, zero, one, custom.
    - Fractional time (6): overlap without integer times, shared endpoint,
      disjoint periods, sub-unit/tiny gaps, uninterrupted activity.
  - Three or more streams (15)
    - Empty/inactive cameras (3): all empty, one empty, one inactive.
    - All-camera agreement (4): interval, singleton, disjoint third camera,
      pairwise overlap without a common period.
    - Later-camera effects (4): remove, split, trim, stagger gaps.
    - More cameras and duplicates (3): four nested, fifth blocks, duplicate stream.
    - Sparse sampling (1): different sampling times across three cameras.

## Diagram legend

The axis shows sample timestamps and interval endpoints. Each `:` column is the
open span between neighboring timestamps; spacing is schematic.

- Period rows: `x` = active at that instant or throughout that span; `-` = inactive.
- Sample rows: `x` = at or above threshold; `.` = below threshold; `-` = no reading.
- `SKIPPED` = diagram exceeds the size limits; fixture data remains listed.

## Part 1: empty_stream

```text
No observations.
threshold = 0.8
camera1 readings = []
expected = []

(empty timeline: no readings; expected [])
```

## Part 1: single_below

```text
One observation, strictly below.
threshold = 0.8
camera1 readings = [(5, 0.2)]
expected = []

time         5
c1 samples   .
expected     -
```

## Part 1: single_equal

```text
One observation, exactly equal.
threshold = 0.8
camera1 readings = [(5, 0.8)]
expected = [(5, 5)]

time         5
c1 samples   x
expected     x
```

## Part 1: single_above

```text
One observation, strictly above.
threshold = 0.8
camera1 readings = [(5, 0.9)]
expected = [(5, 5)]

time         5
c1 samples   x
expected     x
```

## Part 1: never_active

```text
Several readings, none active.
threshold = 0.8
camera1 readings = [(1, 0.0), (3, 0.2), (8, 0.79)]
expected = []

time         1 : 3 : 8
c1 samples   . - . - .
expected     - - - - -
```

## Part 1: always_strictly_above

```text
An all-active stream ends at its final reading.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 1.0), (6, 0.85)]
expected = [(1, 6)]

time         1 : 2 : 6
c1 samples   x - x - x
expected     x x x x x
```

## Part 1: always_exactly_equal

```text
Equality is active throughout.
threshold = 0.8
camera1 readings = [(2, 0.8), (5, 0.8), (9, 0.8)]
expected = [(2, 9)]

time         2 : 5 : 9
c1 samples   x - x - x
expected     x x x x x
```

## Part 1: always_mixed_equal_above

```text
Equality and strict activity share one run.
threshold = 0.8
camera1 readings = [(1, 0.8), (4, 0.95), (6, 0.8), (7, 1.0)]
expected = [(1, 7)]

time         1 : 4 : 6 : 7
c1 samples   x - x - x - x
expected     x x x x x x x
```

## Part 1: singleton_at_start

```text
Only the first index is active.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.1), (3, 0.2)]
expected = [(1, 1)]

time         1 : 2 : 3
c1 samples   x - . - .
expected     x - - - -
```

## Part 1: singleton_in_middle

```text
Only an interior index is active.
threshold = 0.8
camera1 readings = [(1, 0.2), (2, 0.9), (3, 0.2)]
expected = [(2, 2)]

time         1 : 2 : 3
c1 samples   . - x - .
expected     - - x - -
```

## Part 1: singleton_at_end

```text
Only the final index is active.
threshold = 0.8
camera1 readings = [(1, 0.2), (2, 0.1), (3, 0.9)]
expected = [(3, 3)]

time         1 : 2 : 3
c1 samples   . - . - x
expected     - - - - x
```

## Part 1: equal_singleton_at_start

```text
Exactly-threshold singleton at the start.
threshold = 0.8
camera1 readings = [(1, 0.8), (2, 0.2), (3, 0.2)]
expected = [(1, 1)]

time         1 : 2 : 3
c1 samples   x - . - .
expected     x - - - -
```

## Part 1: equal_singleton_in_middle

```text
Exactly-threshold singleton in the middle.
threshold = 0.8
camera1 readings = [(1, 0.2), (2, 0.8), (3, 0.2)]
expected = [(2, 2)]

time         1 : 2 : 3
c1 samples   . - x - .
expected     - - x - -
```

## Part 1: equal_singleton_at_end

```text
Exactly-threshold singleton at the end.
threshold = 0.8
camera1 readings = [(1, 0.2), (2, 0.2), (3, 0.8)]
expected = [(3, 3)]

time         1 : 2 : 3
c1 samples   . - . - x
expected     - - - - x
```

## Part 1: multi_run_at_start

```text
Two active readings followed by an inactive one.
threshold = 0.8
camera1 readings = [(1, 0.9), (3, 0.95), (7, 0.2)]
expected = [(1, 3)]

time         1 : 3 : 7
c1 samples   x - x - .
expected     x x x - -
```

## Part 1: multi_run_in_middle

```text
An active run bracketed by inactive readings.
threshold = 0.8
camera1 readings = [(1, 0.2), (3, 0.9), (5, 1.0), (7, 0.2)]
expected = [(3, 5)]

time         1 : 3 : 5 : 7
c1 samples   . - x - x - .
expected     - - x x x - -
```

## Part 1: multi_run_at_end

```text
An active run reaches the end of input.
threshold = 0.8
camera1 readings = [(1, 0.2), (3, 0.9), (5, 1.0)]
expected = [(3, 5)]

time         1 : 3 : 5
c1 samples   . - x - x
expected     - - x x x
```

## Part 1: equal_multi_run_in_middle

```text
A multi-reading run can consist entirely of equality.
threshold = 0.8
camera1 readings = [(1, 0.2), (3, 0.8), (5, 0.8), (7, 0.2)]
expected = [(3, 5)]

time         1 : 3 : 5 : 7
c1 samples   . - x - x - .
expected     - - x x x - -
```

## Part 1: equal_edges_strict_interior

```text
Equality supplies both endpoints.
threshold = 0.8
camera1 readings = [(1, 0.2), (2, 0.8), (4, 0.95), (6, 0.8), (7, 0.2)]
expected = [(2, 6)]

time         1 : 2 : 4 : 6 : 7
c1 samples   . - x - x - x - .
expected     - - x x x x x - -
```

## Part 1: equal_bridge_between_above

```text
An exactly-threshold reading does not split a run.
threshold = 0.8
camera1 readings = [(1, 0.9), (3, 0.8), (5, 0.9)]
expected = [(1, 5)]

time         1 : 3 : 5
c1 samples   x - x - x
expected     x x x x x
```

## Part 1: one_below_splits_runs

```text
One below-threshold observation really splits activity.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.8), (3, 0.79), (4, 0.8), (5, 0.9)]
expected = [(1, 2), (4, 5)]

time         1 : 2 : 3 : 4 : 5
c1 samples   x - x - . - x - x
expected     x x x - - - x x x
```

## Part 1: alternating_starts_active

```text
Alternating readings with both edges active.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.2), (3, 0.8), (4, 0.2), (5, 1.0)]
expected = [(1, 1), (3, 3), (5, 5)]

time         1 : 2 : 3 : 4 : 5
c1 samples   x - . - x - . - x
expected     x - - - x - - - x
```

## Part 1: alternating_starts_inactive

```text
Alternating readings with both edges inactive.
threshold = 0.8
camera1 readings = [(1, 0.2), (2, 0.8), (3, 0.2), (4, 0.9), (5, 0.2)]
expected = [(2, 2), (4, 4)]

time         1 : 2 : 3 : 4 : 5
c1 samples   . - x - . - x - .
expected     - - x - - - x - -
```

## Part 1: mixed_run_lengths

```text
Singletons, longer runs, and different inactive-gap lengths.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.2), (3, 0.8), (4, 0.9), (5, 1.0), (6, 0.2), (7, 0.2), (8, 0.9), (9, 0.2), (10, 0.8), (12, 0.9)]
expected = [(1, 1), (3, 5), (8, 8), (10, 12)]

time         1 : 2 : 3 : 4 : 5 : 6 : 7 : 8 : 9 : 10 : 12
c1 samples   x - . - x - x - x - . - . - x - . -  x -  x
expected     x - - - x x x x x - - - - - x - - -  x x  x
```

## Part 1: many_inactive_between_runs

```text
Several consecutive inactive readings create one separation.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.9), (3, 0.1), (4, 0.2), (5, 0.0), (6, 0.9), (7, 0.9)]
expected = [(1, 2), (6, 7)]

time         1 : 2 : 3 : 4 : 5 : 6 : 7
c1 samples   x - x - . - . - . - x - x
expected     x x x - - - - - - - x x x
```

## Part 1: last_active_not_first_inactive

```text
The period ends at 5, not the next reading at 100.
threshold = 0.8
camera1 readings = [(2, 0.9), (5, 0.9), (100, 0.2)]
expected = [(2, 5)]

time         2 : 5 : 100
c1 samples   x - x -   .
expected     x x x -   -
```

## Part 1: first_active_not_previous_inactive

```text
The period starts at 100, not the previous reading.
threshold = 0.8
camera1 readings = [(1, 0.2), (100, 0.9), (101, 0.9)]
expected = [(100, 101)]

time         1 : 100 : 101
c1 samples   . -   x -   x
expected     - -   x x   x
```

## Part 1: sparse_active_readings_stay_one_run

```text
No maximum allowed gap between adjacent active readings.
threshold = 0.8
camera1 readings = [(1, 0.9), (100, 0.8), (10000, 0.9)]
expected = [(1, 10000)]

time         1 : 100 : 10000
c1 samples   x -   x -     x
expected     x x   x x     x
```

## Part 1: sparse_inactive_reading_splits

```text
Sparse timestamps do not erase a below-threshold reading.
threshold = 0.8
camera1 readings = [(1, 0.9), (100, 0.2), (10000, 0.9)]
expected = [(1, 1), (10000, 10000)]

time         1 : 100 : 10000
c1 samples   x -   . -     x
expected     x -   - -     x
```

## Part 1: timestamp_zero

```text
Timestamp zero is a valid start.
threshold = 0.8
camera1 readings = [(0, 0.8), (1, 0.9), (2, 0.2)]
expected = [(0, 1)]

time         0 : 1 : 2
c1 samples   x - x - .
expected     x x x - -
```

## Part 1: large_timestamps

```text
Return timestamp values, not list indices.
threshold = 0.8
camera1 readings = [(1000000000000, 0.8), (1000000000007, 0.9), (1000000000100, 0.2)]
expected = [(1000000000000, 1000000000007)]

time         1000000000000 : 1000000000007 : 1000000000100
c1 samples               x -             x -             .
expected                 x x             x -             -
```

## Part 1: threshold_zero

```text
At threshold zero, even zero intensity is active.
threshold = 0.0
camera1 readings = [(1, 0.0), (4, 0.3), (8, 1.0)]
expected = [(1, 8)]

time         1 : 4 : 8
c1 samples   x - x - x
expected     x x x x x
```

## Part 1: threshold_one_none

```text
Below one is inactive when threshold is one.
threshold = 1.0
camera1 readings = [(1, 0.0), (2, 0.9), (3, 0.999999)]
expected = []

time         1 : 2 : 3
c1 samples   . - . - .
expected     - - - - -
```

## Part 1: threshold_one_all

```text
Equality at the highest valid threshold.
threshold = 1.0
camera1 readings = [(1, 1.0), (2, 1.0), (5, 1.0)]
expected = [(1, 5)]

time         1 : 2 : 5
c1 samples   x - x - x
expected     x x x x x
```

## Part 1: threshold_one_mixed

```text
Only exact ones qualify at threshold one.
threshold = 1.0
camera1 readings = [(1, 1.0), (2, 0.99), (3, 1.0), (4, 1.0), (5, 0.0)]
expected = [(1, 1), (3, 4)]

time         1 : 2 : 3 : 4 : 5
c1 samples   x - . - x - x - .
expected     x - - - x x x - -
```

## Part 1: custom_threshold

```text
The threshold is an argument, not a hardcoded 0.8.
threshold = 0.5
camera1 readings = [(1, 0.49), (2, 0.5), (3, 0.51), (4, 0.49)]
expected = [(2, 3)]

time         1 : 2 : 3 : 4
c1 samples   . - x - x - .
expected     - - x x x - -
```

## Part 1: close_to_threshold

```text
Close values on both sides; no tolerance is specified.
threshold = 0.8
camera1 readings = [(1, 0.799999), (2, 0.8), (3, 0.800001), (4, 0.799999)]
expected = [(2, 3)]

time         1 : 2 : 3 : 4
c1 samples   . - x - x - .
expected     - - x x x - -
```

## Part 1: adjacent_runs_preserved_in_part1

```text
The inactive reading at 3.5 creates a positive gap, preserved in both parts.
threshold = 0.8
camera1 readings = [(1, 0.9), (3, 0.8), (3.5, 0.2), (4, 0.9), (6, 0.8)]
expected = [(1, 3), (4, 6)]

time         1 : 3 : 3.5 : 4 : 6
c1 samples   x - x -   . - x - x
expected     x x x -   - - x x x
```

## Part 1: adjacent_singletons_preserved_in_part1

```text
An inactive reading at 2.5 separates two singleton periods in continuous time.
threshold = 0.8
camera1 readings = [(2, 0.8), (2.5, 0.2), (3, 0.9)]
expected = [(2, 2), (3, 3)]

time         2 : 2.5 : 3
c1 samples   x -   . - x
expected     x -   - - x
```

## Part 1: fractional_active_endpoints

```text
Qualifying timestamps may be fractional; preserve the exact endpoints.
threshold = 0.8
camera1 readings = [(0.125, 0.8), (0.5, 0.9), (0.75, 0.2)]
expected = [(0.125, 0.5)]

time         0.125 : 0.5 : 0.75
c1 samples       x -   x -    .
expected         x x   x -    -
```

## Part 1: fractional_gap_between_runs

```text
A positive gap smaller than one time unit still separates runs.
threshold = 0.8
camera1 readings = [(1, 0.9), (1.25, 0.8), (1.3125, 0.2), (1.375, 0.9), (1.5, 0.8)]
expected = [(1, 1.25), (1.375, 1.5)]

time         1 : 1.25 : 1.3125 : 1.375 : 1.5
c1 samples   x -    x -      . -     x -   x
expected     x x    x -      - -     x x   x
```

## Part 1: tiny_positive_gap_between_runs

```text
Even a very small positive gap is retained; there is no timestamp tolerance.
threshold = 0.8
camera1 readings = [(0, 0.9), (1, 0.9), (1.00000000005, 0.2), (1.0000000001, 0.8), (2, 0.9)]
expected = [(0, 1), (1.0000000001, 2)]

time         0 : 1 : 1.00000000005 : 1.0000000001 : 2
c1 samples   x - x -             . -            x - x
expected     x x x -             - -            x x x
```

## Part 1: interview_example

```text
Two active runs separated by a below-threshold reading.
threshold = 0.8
camera1 readings = [(1, 0.4), (5, 0.2), (11, 0.9), (15, 0.9), (17, 0.8), (20, 0.3), (27, 0.9), (31, 1.0), (36, 0.8)]
expected = [(11, 17), (27, 36)]

time         1 : 5 : 11 : 15 : 17 : 20 : 27 : 31 : 36
c1 samples   . - . -  x -  x -  x -  . -  x -  x -  x
expected     - - - -  x x  x x  x -  - -  x x  x x  x
```

## Part 2: no_cameras

```text
The defined empty-camera result is [].
threshold = 0.8
camera_streams = []
expected = []

(empty timeline: no readings; expected [])
```

## Part 2: one_empty_camera

```text
One camera with no observations.
threshold = 0.8
camera1 readings = []
camera1 declared periods = []
expected = []

(empty timeline: no readings; expected [])
```

## Part 2: one_camera_one_run

```text
One camera is sufficient; no second camera is required.
threshold = 0.8
camera1 readings = [(2, 0.9), (6, 0.9)]
camera1 declared periods = [(2, 6)]
expected = [(2, 6)]

time         2 : 6
c1 samples   x - x
c1 periods   x x x
expected     x x x
```

## Part 2: one_camera_separated_runs

```text
One camera preserves separated periods.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.9), (3.5, 0.2), (5, 0.9), (7, 0.9)]
camera1 declared periods = [(1, 2), (5, 7)]
expected = [(1, 2), (5, 7)]

time         1 : 2 : 3.5 : 5 : 7
c1 samples   x - x -   . - x - x
c1 periods   x x x -   - - x x x
expected     x x x -   - - x x x
```

## Part 2: one_camera_adjacent_runs

```text
One camera preserves the entire gap (3,4), including the inactive reading at 3.5.
threshold = 0.8
camera1 readings = [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (6, 0.9)]
camera1 declared periods = [(1, 3), (4, 6)]
expected = [(1, 3), (4, 6)]

time         1 : 3 : 3.5 : 4 : 6
c1 samples   x - x -   . - x - x
c1 periods   x x x -   - - x x x
expected     x x x -   - - x x x
```

## Part 2: all_cameras_inactive

```text
Both cameras have readings, but none qualify.
threshold = 0.8
camera1 readings = [(1, 0.1), (4, 0.2)]
camera1 declared periods = []
camera2 readings = [(2, 0.0), (5, 0.7)]
camera2 declared periods = []
expected = []

time         1 : 2 : 4 : 5
c1 samples   . - - - . - -
c1 periods   - - - - - - -
c2 samples   - - . - - - .
c2 periods   - - - - - - -
expected     - - - - - - -
```

## Part 2: one_camera_just_below_threshold

```text
A nearly-active camera still blocks the entire result.
threshold = 0.8
camera1 readings = [(1, 0.9), (5, 0.9)]
camera1 declared periods = [(1, 5)]
camera2 readings = [(1, 0.799999), (5, 0.799999)]
camera2 declared periods = []
expected = []

time         1 : 5
c1 samples   x - x
c1 periods   x x x
c2 samples   . - .
c2 periods   - - -
expected     - - -
```

## Part 2: identical_periods

```text
Identical starts and ends.
threshold = 0.8
camera1 readings = [(2, 0.9), (6, 0.9)]
camera1 declared periods = [(2, 6)]
camera2 readings = [(2, 0.9), (6, 0.9)]
camera2 declared periods = [(2, 6)]
expected = [(2, 6)]

time         2 : 6
c1 samples   x - x
c1 periods   x x x
c2 samples   x - x
c2 periods   x x x
expected     x x x
```

## Part 2: partial_overlap

```text
Starts and ends are staggered.
threshold = 0.8
camera1 readings = [(1, 0.9), (5, 0.9)]
camera1 declared periods = [(1, 5)]
camera2 readings = [(3, 0.9), (8, 0.9)]
camera2 declared periods = [(3, 8)]
expected = [(3, 5)]

time         1 : 3 : 5 : 8
c1 samples   x - - - x - -
c1 periods   x x x x x - -
c2 samples   - - x - - - x
c2 periods   - - x x x x x
expected     - - x x x - -
```

## Part 2: partial_overlap_reversed

```text
The same geometry in the opposite camera order.
threshold = 0.8
camera1 readings = [(3, 0.9), (8, 0.9)]
camera1 declared periods = [(3, 8)]
camera2 readings = [(1, 0.9), (5, 0.9)]
camera2 declared periods = [(1, 5)]
expected = [(3, 5)]

time         1 : 3 : 5 : 8
c1 samples   - - x - - - x
c1 periods   - - x x x x x
c2 samples   x - - - x - -
c2 periods   x x x x x - -
expected     - - x x x - -
```

## Part 2: first_contains_second

```text
Strict containment.
threshold = 0.8
camera1 readings = [(1, 0.9), (10, 0.9)]
camera1 declared periods = [(1, 10)]
camera2 readings = [(3, 0.9), (7, 0.9)]
camera2 declared periods = [(3, 7)]
expected = [(3, 7)]

time         1 : 3 : 7 : 10
c1 samples   x - - - - -  x
c1 periods   x x x x x x  x
c2 samples   - - x - x -  -
c2 periods   - - x x x -  -
expected     - - x x x -  -
```

## Part 2: second_contains_first

```text
Containment with the shorter camera first.
threshold = 0.8
camera1 readings = [(3, 0.9), (7, 0.9)]
camera1 declared periods = [(3, 7)]
camera2 readings = [(1, 0.9), (10, 0.9)]
camera2 declared periods = [(1, 10)]
expected = [(3, 7)]

time         1 : 3 : 7 : 10
c1 samples   - - x - x -  -
c1 periods   - - x x x -  -
c2 samples   x - - - - -  x
c2 periods   x x x x x x  x
expected     - - x x x -  -
```

## Part 2: same_start_different_ends

```text
Equal starts do not imply equal ends.
threshold = 0.8
camera1 readings = [(2, 0.9), (5, 0.9)]
camera1 declared periods = [(2, 5)]
camera2 readings = [(2, 0.9), (8, 0.9)]
camera2 declared periods = [(2, 8)]
expected = [(2, 5)]

time         2 : 5 : 8
c1 samples   x - x - -
c1 periods   x x x - -
c2 samples   x - - - x
c2 periods   x x x x x
expected     x x x - -
```

## Part 2: same_end_different_starts

```text
Equal ends do not imply equal starts.
threshold = 0.8
camera1 readings = [(1, 0.9), (7, 0.9)]
camera1 declared periods = [(1, 7)]
camera2 readings = [(4, 0.9), (7, 0.9)]
camera2 declared periods = [(4, 7)]
expected = [(4, 7)]

time         1 : 4 : 7
c1 samples   x - - - x
c1 periods   x x x x x
c2 samples   - - x - x
c2 periods   - - x x x
expected     - - x x x
```

## Part 2: disjoint_first_earlier

```text
First camera finishes before the second starts.
threshold = 0.8
camera1 readings = [(1, 0.9), (3, 0.9)]
camera1 declared periods = [(1, 3)]
camera2 readings = [(6, 0.9), (8, 0.9)]
camera2 declared periods = [(6, 8)]
expected = []

time         1 : 3 : 6 : 8
c1 samples   x - x - - - -
c1 periods   x x x - - - -
c2 samples   - - - - x - x
c2 periods   - - - - x x x
expected     - - - - - - -
```

## Part 2: disjoint_second_earlier

```text
Second camera finishes before the first starts.
threshold = 0.8
camera1 readings = [(6, 0.9), (8, 0.9)]
camera1 declared periods = [(6, 8)]
camera2 readings = [(1, 0.9), (3, 0.9)]
camera2 declared periods = [(1, 3)]
expected = []

time         1 : 3 : 6 : 8
c1 samples   - - - - x - x
c1 periods   - - - - x x x
c2 samples   x - x - - - -
c2 periods   x x x - - - -
expected     - - - - - - -
```

## Part 2: adjacent_cameras_do_not_overlap

```text
The positive gap (2,3) means the cameras never overlap.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.9)]
camera1 declared periods = [(1, 2)]
camera2 readings = [(3, 0.9), (4, 0.9)]
camera2 declared periods = [(3, 4)]
expected = []

time         1 : 2 : 3 : 4
c1 samples   x - x - - - -
c1 periods   x x x - - - -
c2 samples   - - - - x - x
c2 periods   - - - - x x x
expected     - - - - - - -
```

## Part 2: interleaved_but_disjoint

```text
Multiple alternating periods, but no shared active time.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.9), (4.5, 0.2), (7, 0.9), (8, 0.9)]
camera1 declared periods = [(1, 2), (7, 8)]
camera2 readings = [(4, 0.9), (5, 0.9), (7.5, 0.2), (10, 0.9), (11, 0.9)]
camera2 declared periods = [(4, 5), (10, 11)]
expected = []

time         1 : 2 : 4 : 4.5 : 5 : 7 : 7.5 : 8 : 10 : 11
c1 samples   x - x - - -   . - - - x -   - - x -  - -  -
c1 periods   x x x - - -   - - - - x x   x x x -  - -  -
c2 samples   - - - - x -   - - x - - -   . - - -  x -  x
c2 periods   - - - - x x   x x x - - -   - - - -  x x  x
expected     - - - - - -   - - - - - -   - - - -  - -  -
```

## Part 2: different_singletons

```text
Consecutive singletons in different cameras are still disjoint.
threshold = 0.8
camera1 readings = [(3, 0.9)]
camera1 declared periods = [(3, 3)]
camera2 readings = [(4, 0.9)]
camera2 declared periods = [(4, 4)]
expected = []

time         3 : 4
c1 samples   x - -
c1 periods   x - -
c2 samples   - - x
c2 periods   - - x
expected     - - -
```

## Part 2: endpoint_only_overlap

```text
A shared endpoint is a valid singleton overlap.
threshold = 0.8
camera1 readings = [(1, 0.9), (4, 0.9)]
camera1 declared periods = [(1, 4)]
camera2 readings = [(4, 0.9), (7, 0.9)]
camera2 declared periods = [(4, 7)]
expected = [(4, 4)]

time         1 : 4 : 7
c1 samples   x - x - -
c1 periods   x x x - -
c2 samples   - - x - x
c2 periods   - - x x x
expected     - - x - -
```

## Part 2: same_singleton

```text
Two cameras active at the same single instant.
threshold = 0.8
camera1 readings = [(3, 0.9)]
camera1 declared periods = [(3, 3)]
camera2 readings = [(3, 0.9)]
camera2 declared periods = [(3, 3)]
expected = [(3, 3)]

time         3
c1 samples   x
c1 periods   x
c2 samples   x
c2 periods   x
expected     x
```

## Part 2: singleton_inside_period

```text
A singleton strictly inside a longer period.
threshold = 0.8
camera1 readings = [(4, 0.9)]
camera1 declared periods = [(4, 4)]
camera2 readings = [(1, 0.9), (7, 0.9)]
camera2 declared periods = [(1, 7)]
expected = [(4, 4)]

time         1 : 4 : 7
c1 samples   - - x - -
c1 periods   - - x - -
c2 samples   x - - - x
c2 periods   x x x x x
expected     - - x - -
```

## Part 2: singleton_at_period_start

```text
Singleton at the other period's start.
threshold = 0.8
camera1 readings = [(2, 0.9)]
camera1 declared periods = [(2, 2)]
camera2 readings = [(2, 0.9), (7, 0.9)]
camera2 declared periods = [(2, 7)]
expected = [(2, 2)]

time         2 : 7
c1 samples   x - -
c1 periods   x - -
c2 samples   x - x
c2 periods   x x x
expected     x - -
```

## Part 2: singleton_at_period_end

```text
Singleton at the other period's end.
threshold = 0.8
camera1 readings = [(7, 0.9)]
camera1 declared periods = [(7, 7)]
camera2 readings = [(2, 0.9), (7, 0.9)]
camera2 declared periods = [(2, 7)]
expected = [(7, 7)]

time         2 : 7
c1 samples   - - x
c1 periods   - - x
c2 samples   x - x
c2 periods   x x x
expected     - - x
```

## Part 2: one_overlap_on_one_segment

```text
Only one of the second camera's periods overlaps.
threshold = 0.8
camera1 readings = [(2, 0.9), (4, 0.9)]
camera1 declared periods = [(2, 4)]
camera2 readings = [(1, 0.9), (2, 0.9), (4, 0.2), (6, 0.9), (7, 0.9)]
camera2 declared periods = [(1, 2), (6, 7)]
expected = [(2, 2)]

time         1 : 2 : 4 : 6 : 7
c1 samples   - - x - x - - - -
c1 periods   - - x x x - - - -
c2 samples   x - x - . - x - x
c2 periods   x x x - - - x x x
expected     - - x - - - - - -
```

## Part 2: two_endpoint_overlaps_on_one_segment

```text
The positive gap (2,4) separates the two singleton outputs.
threshold = 0.8
camera1 readings = [(2, 0.9), (4, 0.9)]
camera1 declared periods = [(2, 4)]
camera2 readings = [(1, 0.9), (2, 0.9), (3, 0.2), (4, 0.9), (5, 0.9)]
camera2 declared periods = [(1, 2), (4, 5)]
expected = [(2, 2), (4, 4)]

time         1 : 2 : 3 : 4 : 5
c1 samples   - - x - - - x - -
c1 periods   - - x x x x x - -
c2 samples   x - x - . - x - x
c2 periods   x x x - - - x x x
expected     - - x - - - x - -
```

## Part 2: two_wide_overlaps_on_one_segment

```text
Two positive-length overlaps remain separated.
threshold = 0.8
camera1 readings = [(2, 0.9), (10, 0.9)]
camera1 declared periods = [(2, 10)]
camera2 readings = [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (12, 0.9)]
camera2 declared periods = [(1, 4), (7, 12)]
expected = [(2, 4), (7, 10)]

time         1 : 2 : 4 : 5.5 : 7 : 10 : 12
c1 samples   - - x - - -   - - - -  x -  -
c1 periods   - - x x x x   x x x x  x -  -
c2 samples   x - - - x -   . - x -  - -  x
c2 periods   x x x x x -   - - x x  x x  x
expected     - - x x x -   - - x x  x -  -
```

## Part 2: two_adjacent_overlaps_preserve_gap

```text
The positive gap (3,4) must remain even though its endpoints are consecutive integers.
threshold = 0.8
camera1 readings = [(2, 0.9), (4, 0.9)]
camera1 declared periods = [(2, 4)]
camera2 readings = [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (5, 0.9)]
camera2 declared periods = [(1, 3), (4, 5)]
expected = [(2, 3), (4, 4)]

time         1 : 2 : 3 : 3.5 : 4 : 5
c1 samples   - - x - - -   - - x - -
c1 periods   - - x x x x   x x x - -
c2 samples   x - - - x -   . - x - x
c2 periods   x x x x x -   - - x x x
expected     - - x x x -   - - x - -
```

## Part 2: two_adjacent_singleton_overlaps_preserve_gap

```text
Distinct singleton overlaps do not include any time between them.
threshold = 0.8
camera1 readings = [(2, 0.9), (3, 0.9)]
camera1 declared periods = [(2, 3)]
camera2 readings = [(1, 0.9), (2, 0.9), (2.5, 0.2), (3, 0.9), (4, 0.9)]
camera2 declared periods = [(1, 2), (3, 4)]
expected = [(2, 2), (3, 3)]

time         1 : 2 : 2.5 : 3 : 4
c1 samples   - - x -   - - x - -
c1 periods   - - x x   x x x - -
c2 samples   x - x -   . - x - x
c2 periods   x x x -   - - x x x
expected     - - x -   - - x - -
```

## Part 2: inactive_reading_prevents_merge

```text
The inactive reading at 3 separates runs; preserve the whole gap (2,4).
threshold = 0.8
camera1 readings = [(2, 0.9), (5, 0.9)]
camera1 declared periods = [(2, 5)]
camera2 readings = [(1, 0.9), (2, 0.9), (3, 0.2), (4, 0.9), (6, 0.9)]
camera2 declared periods = [(1, 2), (4, 6)]
expected = [(2, 2), (4, 5)]

time         1 : 2 : 3 : 4 : 5 : 6
c1 samples   - - x - - - - - x - -
c1 periods   - - x x x x x x x - -
c2 samples   x - x - . - x - - - x
c2 periods   x x x - - - x x x x x
expected     - - x - - - x x x - -
```

## Part 2: three_overlaps_on_one_segment

```text
One long run is reused for three separated overlaps.
threshold = 0.8
camera1 readings = [(1, 0.9), (12, 0.9)]
camera1 declared periods = [(1, 12)]
camera2 readings = [(0, 0.9), (2, 0.9), (3.5, 0.2), (5, 0.9), (7, 0.9), (8.5, 0.2), (10, 0.9), (14, 0.9)]
camera2 declared periods = [(0, 2), (5, 7), (10, 14)]
expected = [(1, 2), (5, 7), (10, 12)]

time         0 : 1 : 2 : 3.5 : 5 : 7 : 8.5 : 10 : 12 : 14
c1 samples   - - x - - -   - - - - - -   - -  - -  x -  -
c1 periods   - - x x x x   x x x x x x   x x  x x  x -  -
c2 samples   x - - - x -   . - x - x -   . -  x -  - -  x
c2 periods   x x x x x -   - - x x x -   - -  x x  x x  x
expected     - - x x x -   - - x x x -   - -  x x  x -  -
```

## Part 2: three_adjacent_overlaps_preserve_gaps

```text
Both positive gaps in a chain of three overlap pieces remain.
threshold = 0.8
camera1 readings = [(1, 0.9), (9, 0.9)]
camera1 declared periods = [(1, 9)]
camera2 readings = [(0, 0.9), (2, 0.9), (2.5, 0.2), (3, 0.9), (5, 0.9), (5.5, 0.2), (6, 0.9), (10, 0.9)]
camera2 declared periods = [(0, 2), (3, 5), (6, 10)]
expected = [(1, 2), (3, 5), (6, 9)]

time         0 : 1 : 2 : 2.5 : 3 : 5 : 5.5 : 6 : 9 : 10
c1 samples   - - x - - -   - - - - - -   - - - - x -  -
c1 periods   - - x x x x   x x x x x x   x x x x x -  -
c2 samples   x - - - x -   . - x - x -   . - x - - -  x
c2 periods   x x x x x -   - - x x x -   - - x x x x  x
expected     - - x x x -   - - x x x -   - - x x x -  -
```

## Part 2: small_gaps_around_larger_gap

```text
Preserve the smaller gaps (3,4) and (9,10), as well as the larger gap (5,8).
threshold = 0.8
camera1 readings = [(2, 0.9), (12, 0.9)]
camera1 declared periods = [(2, 12)]
camera2 readings = [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (5, 0.9), (6.5, 0.2), (8, 0.9), (9, 0.9), (9.5, 0.2), (10, 0.9), (14, 0.9)]
camera2 declared periods = [(1, 3), (4, 5), (8, 9), (10, 14)]
expected = [(2, 3), (4, 5), (8, 9), (10, 12)]

time         1 : 2 : 3 : 3.5 : 4 : 5 : 6.5 : 8 : 9 : 9.5 : 10 : 12 : 14
c1 samples   - - x - - -   - - - - - -   - - - - - -   - -  - -  x -  -
c1 periods   - - x x x x   x x x x x x   x x x x x x   x x  x x  x -  -
c2 samples   x - - - x -   . - x - x -   . - x - x -   . -  x -  - -  x
c2 periods   x x x x x -   - - x x x -   - - x x x -   - -  x x  x x  x
expected     - - x x x -   - - x x x -   - - x x x -   - -  x x  x -  -
```

## Part 2: later_chain_after_earlier_chain

```text
Both long periods have multiple overlaps whose positive gaps remain.
threshold = 0.8
camera1 readings = [(2, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)]
camera1 declared periods = [(2, 4), (7, 10)]
camera2 readings = [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (5, 0.9), (5.5, 0.2), (6, 0.9), (8, 0.9), (8.5, 0.2), (9, 0.9), (11, 0.9)]
camera2 declared periods = [(1, 3), (4, 5), (6, 8), (9, 11)]
expected = [(2, 3), (4, 4), (7, 8), (9, 10)]

time         1 : 2 : 3 : 3.5 : 4 : 5 : 5.5 : 6 : 7 : 8 : 8.5 : 9 : 10 : 11
c1 samples   - - x - - -   - - x - - -   . - - - x - - -   - - - -  x -  -
c1 periods   - - x x x x   x x x - - -   - - - - x x x x   x x x x  x -  -
c2 samples   x - - - x -   . - x - x -   . - x - - - x -   . - x -  - -  x
c2 periods   x x x x x -   - - x x x -   - - x x x x x -   - - x x  x x  x
expected     - - x x x -   - - x - - -   - - - - x x x -   - - x x  x -  -
```

## Part 2: both_cameras_fragmented_chains

```text
Gaps from either camera remain, including around singleton overlaps.
threshold = 0.8
camera1 readings = [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (6, 0.9), (7.5, 0.2), (9, 0.9), (11, 0.9), (11.5, 0.2), (12, 0.9), (15, 0.9)]
camera1 declared periods = [(1, 3), (4, 6), (9, 11), (12, 15)]
camera2 readings = [(2, 0.9), (4, 0.9), (4.5, 0.2), (5, 0.9), (7, 0.9), (8.5, 0.2), (10, 0.9), (12, 0.9), (12.5, 0.2), (13, 0.9), (14, 0.9)]
camera2 declared periods = [(2, 4), (5, 7), (10, 12), (13, 14)]
expected = [(2, 3), (4, 4), (5, 6), (10, 11), (12, 12), (13, 14)]

time         1 : 2 : 3 : 3.5 : 4 : 4.5 : 5 : 6 : 7 : 7.5 : 8.5 : 9 : 10 : 11 : 11.5 : 12 : 12.5 : 13 : 14 : 15
c1 samples   x - - - x -   . - x -   - - - - x - - -   . -   - - x -  - -  x -    . -  x -    - -  - -  - -  x
c1 periods   x x x x x -   - - x x   x x x x x - - -   - -   - - x x  x x  x -    - -  x x    x x  x x  x x  x
c2 samples   - - x - - -   - - x -   . - x - - - x -   - -   . - - -  x -  - -    - -  x -    . -  x -  x -  -
c2 periods   - - x x x x   x x x -   - - x x x x x -   - -   - - - -  x x  x x    x x  x -    - -  x x  x -  -
expected     - - x x x -   - - x -   - - x x x - - -   - -   - - - -  x x  x -    - -  x -    - -  x x  x -  -
```

## Part 2: alternating_which_camera_ends_first

```text
Several overlaps; the earlier ending camera changes repeatedly.
threshold = 0.8
camera1 readings = [(1, 0.9), (5, 0.9), (6.5, 0.2), (8, 0.9), (12, 0.9), (13.5, 0.2), (15, 0.9), (19, 0.9)]
camera1 declared periods = [(1, 5), (8, 12), (15, 19)]
camera2 readings = [(3, 0.9), (9, 0.9), (10, 0.2), (11, 0.9), (16, 0.9), (17, 0.2), (18, 0.9), (22, 0.9)]
camera2 declared periods = [(3, 9), (11, 16), (18, 22)]
expected = [(3, 5), (8, 9), (11, 12), (15, 16), (18, 19)]

time         1 : 3 : 5 : 6.5 : 8 : 9 : 10 : 11 : 12 : 13.5 : 15 : 16 : 17 : 18 : 19 : 22
c1 samples   x - - - x -   . - x - - -  - -  - -  x -    . -  x -  - -  - -  - -  x -  -
c1 periods   x x x x x -   - - x x x x  x x  x x  x -    - -  x x  x x  x x  x x  x -  -
c2 samples   - - x - - -   - - - - x -  . -  x -  - -    - -  - -  x -  . -  x -  - -  x
c2 periods   - - x x x x   x x x x x -  - -  x x  x x    x x  x x  x -  - -  x x  x x  x
expected     - - x x x -   - - x x x -  - -  x x  x -    - -  x x  x -  - -  x x  x -  -
```

## Part 2: early_nonmatches_then_overlap

```text
Several early periods do not intersect anything.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.9), (3.5, 0.2), (5, 0.9), (6, 0.9), (7.5, 0.2), (9, 0.9), (12, 0.9)]
camera1 declared periods = [(1, 2), (5, 6), (9, 12)]
camera2 readings = [(10, 0.9), (14, 0.9)]
camera2 declared periods = [(10, 14)]
expected = [(10, 12)]

time         1 : 2 : 3.5 : 5 : 6 : 7.5 : 9 : 10 : 12 : 14
c1 samples   x - x -   . - x - x -   . - x -  - -  x -  -
c1 periods   x x x -   - - x x x -   - - x x  x x  x -  -
c2 samples   - - - -   - - - - - -   - - - -  x -  - -  x
c2 periods   - - - -   - - - - - -   - - - -  x x  x x  x
expected     - - - -   - - - - - -   - - - -  x x  x -  -
```

## Part 2: overlap_then_trailing_nonmatches

```text
Only the first period intersects; later ones must not leak into output.
threshold = 0.8
camera1 readings = [(1, 0.9), (4, 0.9), (6, 0.2), (8, 0.9), (9, 0.9), (10.5, 0.2), (12, 0.9), (13, 0.9)]
camera1 declared periods = [(1, 4), (8, 9), (12, 13)]
camera2 readings = [(2, 0.9), (3, 0.9)]
camera2 declared periods = [(2, 3)]
expected = [(2, 3)]

time         1 : 2 : 3 : 4 : 6 : 8 : 9 : 10.5 : 12 : 13
c1 samples   x - - - - - x - . - x - x -    . -  x -  x
c1 periods   x x x x x x x - - - x x x -    - -  x x  x
c2 samples   - - x - x - - - - - - - - -    - -  - -  -
c2 periods   - - x x x - - - - - - - - -    - -  - -  -
expected     - - x x x - - - - - - - - -    - -  - -  -
```

## Part 2: equal_ends_then_later_overlap

```text
Equal first endpoints followed by more work.
threshold = 0.8
camera1 readings = [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)]
camera1 declared periods = [(1, 4), (7, 10)]
camera2 readings = [(2, 0.9), (4, 0.9), (6, 0.2), (8, 0.9), (12, 0.9)]
camera2 declared periods = [(2, 4), (8, 12)]
expected = [(2, 4), (8, 10)]

time         1 : 2 : 4 : 5.5 : 6 : 7 : 8 : 10 : 12
c1 samples   x - - - x -   . - - - x - - -  x -  -
c1 periods   x x x x x -   - - - - x x x x  x -  -
c2 samples   - - x - x -   - - . - - - x -  - -  x
c2 periods   - - x x x -   - - - - - - x x  x x  x
expected     - - x x x -   - - - - - - x x  x -  -
```

## Part 2: repeated_endpoint_only_overlaps

```text
Four endpoint-only overlaps across two long periods.
threshold = 0.8
camera1 readings = [(2, 0.9), (4, 0.9), (6, 0.2), (8, 0.9), (10, 0.9)]
camera1 declared periods = [(2, 4), (8, 10)]
camera2 readings = [(1, 0.9), (2, 0.9), (3, 0.2), (4, 0.9), (8, 0.9), (9, 0.2), (10, 0.9), (11, 0.9)]
camera2 declared periods = [(1, 2), (4, 8), (10, 11)]
expected = [(2, 2), (4, 4), (8, 8), (10, 10)]

time         1 : 2 : 3 : 4 : 6 : 8 : 9 : 10 : 11
c1 samples   - - x - - - x - . - x - - -  x -  -
c1 periods   - - x x x x x - - - x x x x  x -  -
c2 samples   x - x - . - x - - - x - . -  x -  x
c2 periods   x x x - - - x x x x x - - -  x x  x
expected     - - x - - - x - - - x - - -  x -  -
```

## Part 2: staggered_gaps_form_larger_gap

```text
The output gap (2,5) combines gaps from different cameras; neither alone spans it.
threshold = 0.8
camera1 readings = [(0, 0.9), (2, 0.9), (3, 0.2), (4, 0.9), (8, 0.9)]
camera1 declared periods = [(0, 2), (4, 8)]
camera2 readings = [(0, 0.9), (3, 0.9), (4, 0.2), (5, 0.9), (8, 0.9)]
camera2 declared periods = [(0, 3), (5, 8)]
expected = [(0, 2), (5, 8)]

time         0 : 2 : 3 : 4 : 5 : 8
c1 samples   x - x - . - x - - - x
c1 periods   x x x - - - x x x x x
c2 samples   x - - - x - . - x - x
c2 periods   x x x x x - - - x x x
expected     x x x - - - - - x x x
```

## Part 2: overlap_without_shared_reading_timestamps

```text
No raw timestamp is shared; their inferred periods still overlap.
threshold = 0.8
camera1 readings = [(1, 0.9), (6, 0.9)]
camera1 declared periods = [(1, 6)]
camera2 readings = [(4, 0.9), (10, 0.9)]
camera2 declared periods = [(4, 10)]
expected = [(4, 6)]

time         1 : 4 : 6 : 10
c1 samples   x - - - x -  -
c1 periods   x x x x x -  -
c2 samples   - - x - - -  x
c2 periods   - - x x x x  x
expected     - - x x x -  -
```

## Part 2: timestamp_zero_overlap

```text
Zero is a valid singleton overlap.
threshold = 0.8
camera1 readings = [(0, 0.9), (3, 0.9)]
camera1 declared periods = [(0, 3)]
camera2 readings = [(0, 0.9)]
camera2 declared periods = [(0, 0)]
expected = [(0, 0)]

time         0 : 3
c1 samples   x - x
c1 periods   x x x
c2 samples   x - -
c2 periods   x - -
expected     x - -
```

## Part 2: large_timestamps_overlap

```text
Large timestamp values must be preserved exactly.
threshold = 0.8
camera1 readings = [(1000000000000, 0.9), (1000000000010, 0.9)]
camera1 declared periods = [(1000000000000, 1000000000010)]
camera2 readings = [(1000000000004, 0.9), (1000000000020, 0.9)]
camera2 declared periods = [(1000000000004, 1000000000020)]
expected = [(1000000000004, 1000000000010)]

time         1000000000000 : 1000000000004 : 1000000000010 : 1000000000020
c1 samples               x -             - -             x -             -
c1 periods               x x             x x             x -             -
c2 samples               - -             x -             - -             x
c2 periods               - -             x x             x x             x
expected                 - -             x x             x -             -
```

## Part 2: threshold_equality_at_overlap_boundaries

```text
Both overlap boundaries are exactly threshold in their own camera.
threshold = 0.8
camera1 readings = [(0, 0.2), (1, 0.9), (4, 0.8), (6, 0.2)]
camera1 declared periods = [(1, 4)]
camera2 readings = [(1, 0.2), (2, 0.8), (5, 0.9), (7, 0.2)]
camera2 declared periods = [(2, 5)]
expected = [(2, 4)]

time         0 : 1 : 2 : 4 : 5 : 6 : 7
c1 samples   . - x - - - x - - - . - -
c1 periods   - - x x x x x - - - - - -
c2 samples   - - . - x - - - x - - - .
c2 periods   - - - - x x x x x - - - -
expected     - - - - x x x - - - - - -
```

## Part 2: exact_threshold_singletons

```text
Equality alone supplies a shared singleton.
threshold = 0.8
camera1 readings = [(1, 0.2), (3, 0.8), (5, 0.2)]
camera1 declared periods = [(3, 3)]
camera2 readings = [(2, 0.2), (3, 0.8), (4, 0.2)]
camera2 declared periods = [(3, 3)]
expected = [(3, 3)]

time         1 : 2 : 3 : 4 : 5
c1 samples   . - - - x - - - .
c1 periods   - - - - x - - - -
c2 samples   - - . - x - . - -
c2 periods   - - - - x - - - -
expected     - - - - x - - - -
```

## Part 2: threshold_zero_different_extents

```text
All readings qualify, but camera time extents still constrain overlap.
threshold = 0.0
camera1 readings = [(1, 0.0), (5, 0.2)]
camera1 declared periods = [(1, 5)]
camera2 readings = [(3, 0.0), (8, 1.0)]
camera2 declared periods = [(3, 8)]
expected = [(3, 5)]

time         1 : 3 : 5 : 8
c1 samples   x - - - x - -
c1 periods   x x x x x - -
c2 samples   - - x - - - x
c2 periods   - - x x x x x
expected     - - x x x - -
```

## Part 2: threshold_one_split_overlap

```text
Only readings equal to one qualify, including isolated boundary overlaps.
threshold = 1.0
camera1 readings = [(1, 1.0), (2, 1.0), (3, 0.99), (4, 1.0), (5, 1.0)]
camera1 declared periods = [(1, 2), (4, 5)]
camera2 readings = [(1, 0.9), (2, 1.0), (4, 1.0), (5, 0.9)]
camera2 declared periods = [(2, 4)]
expected = [(2, 2), (4, 4)]

time         1 : 2 : 3 : 4 : 5
c1 samples   x - x - . - x - x
c1 periods   x x x - - - x x x
c2 samples   . - x - - - x - .
c2 periods   - - x x x x x - -
expected     - - x - - - x - -
```

## Part 2: custom_threshold_all_cameras

```text
A nondefault threshold applies to every stream.
threshold = 0.5
camera1 readings = [(1, 0.49), (2, 0.5), (4, 0.6), (6, 0.49)]
camera1 declared periods = [(2, 4)]
camera2 readings = [(2, 0.49), (3, 0.5), (5, 0.9), (7, 0.49)]
camera2 declared periods = [(3, 5)]
expected = [(3, 4)]

time         1 : 2 : 3 : 4 : 5 : 6 : 7
c1 samples   . - x - - - x - - - . - -
c1 periods   - - x x x x x - - - - - -
c2 samples   - - . - x - - - x - - - .
c2 periods   - - - - x x x x x - - - -
expected     - - - - x x x - - - - - -
```

## Part 2: fractional_overlap_without_integer_timestamp

```text
A nonempty common interval can contain no integer timestamp.
threshold = 0.8
camera1 readings = [(0.125, 0.9), (0.625, 0.9)]
camera1 declared periods = [(0.125, 0.625)]
camera2 readings = [(0.375, 0.8), (0.875, 0.9)]
camera2 declared periods = [(0.375, 0.875)]
expected = [(0.375, 0.625)]

time         0.125 : 0.375 : 0.625 : 0.875
c1 samples       x -     - -     x -     -
c1 periods       x x     x x     x -     -
c2 samples       - -     x -     - -     x
c2 periods       - -     x x     x x     x
expected         - -     x x     x -     -
```

## Part 2: fractional_shared_endpoint

```text
Closed intervals meeting at a fractional endpoint share exactly that instant.
threshold = 0.8
camera1 readings = [(0.125, 0.9), (0.375, 0.9)]
camera1 declared periods = [(0.125, 0.375)]
camera2 readings = [(0.375, 0.8), (0.625, 0.9)]
camera2 declared periods = [(0.375, 0.625)]
expected = [(0.375, 0.375)]

time         0.125 : 0.375 : 0.625
c1 samples       x -     x -     -
c1 periods       x x     x -     -
c2 samples       - -     x -     x
c2 periods       - -     x x     x
expected         - -     x -     -
```

## Part 2: fractional_disjoint_cameras

```text
A sub-unit gap between cameras is still disjoint; timestamps are not rounded.
threshold = 0.8
camera1 readings = [(0.125, 0.9), (0.375, 0.9)]
camera1 declared periods = [(0.125, 0.375)]
camera2 readings = [(0.5, 0.9), (0.875, 0.9)]
camera2 declared periods = [(0.5, 0.875)]
expected = []

time         0.125 : 0.375 : 0.5 : 0.875
c1 samples       x -     x -   - -     -
c1 periods       x x     x -   - -     -
c2 samples       - -     - -   x -     x
c2 periods       - -     - -   x x     x
expected         - -     - -   - -     -
```

## Part 2: fractional_gap_preserved

```text
Activity in the first camera cannot fill the second camera's fractional gap.
threshold = 0.8
camera1 readings = [(0, 0.9), (2, 0.9)]
camera1 declared periods = [(0, 2)]
camera2 readings = [(1, 0.9), (1.25, 0.8), (1.3125, 0.2), (1.375, 0.9), (1.5, 0.8)]
camera2 declared periods = [(1, 1.25), (1.375, 1.5)]
expected = [(1, 1.25), (1.375, 1.5)]

time         0 : 1 : 1.25 : 1.3125 : 1.375 : 1.5 : 2
c1 samples   x - - -    - -      - -     - -   - - x
c1 periods   x x x x    x x      x x     x x   x x x
c2 samples   - - x -    x -      . -     x -   x - -
c2 periods   - - x x    x -      - -     x x   x - -
expected     - - x x    x -      - -     x x   x - -
```

## Part 2: tiny_positive_gap_preserved

```text
A very small positive gap must not be filled by a tolerance or adjacency rule.
threshold = 0.8
camera1 readings = [(0, 0.9), (3, 0.9)]
camera1 declared periods = [(0, 3)]
camera2 readings = [(0, 0.9), (1, 0.9), (1.00000000005, 0.2), (1.0000000001, 0.8), (2, 0.9)]
camera2 declared periods = [(0, 1), (1.0000000001, 2)]
expected = [(0, 1), (1.0000000001, 2)]

time         0 : 1 : 1.00000000005 : 1.0000000001 : 2 : 3
c1 samples   x - - -             - -            - - - - x
c1 periods   x x x x             x x            x x x x x
c2 samples   x - x -             . -            x - x - -
c2 periods   x x x -             - -            x x x - -
expected     x x x -             - -            x x x - -
```

## Part 2: uninterrupted_fractional_period

```text
Interior sample boundaries introduce no gap or redundant output split.
threshold = 0.8
camera1 readings = [(0, 0.9), (0.25, 0.8), (0.75, 0.9), (1, 0.9)]
camera1 declared periods = [(0, 1)]
camera2 readings = [(0.125, 0.9), (0.5, 0.8), (0.875, 0.9)]
camera2 declared periods = [(0.125, 0.875)]
expected = [(0.125, 0.875)]

time         0 : 0.125 : 0.25 : 0.5 : 0.75 : 0.875 : 1
c1 samples   x -     - -    x -   - -    x -     - - x
c1 periods   x x     x x    x x   x x    x x     x x x
c2 samples   - -     x -    - -   x -    - -     x - -
c2 periods   - -     x x    x x   x x    x x     x - -
expected     - -     x x    x x   x x    x x     x - -
```

## Part 2: all_cameras_empty

```text
Several cameras, all empty.
threshold = 0.8
camera1 readings = []
camera1 declared periods = []
camera2 readings = []
camera2 declared periods = []
camera3 readings = []
camera3 declared periods = []
expected = []

(empty timeline: no readings; expected [])
```

## Part 2: empty_camera_among_active

```text
Every camera is required, including an empty one.
threshold = 0.8
camera1 readings = [(1, 0.9), (5, 0.9)]
camera1 declared periods = [(1, 5)]
camera2 readings = []
camera2 declared periods = []
camera3 readings = [(2, 0.9), (4, 0.9)]
camera3 declared periods = [(2, 4)]
expected = []

time         1 : 2 : 4 : 5
c1 samples   x - - - - - x
c1 periods   x x x x x x x
c2 samples   - - - - - - -
c2 periods   - - - - - - -
c3 samples   - - x - x - -
c3 periods   - - x x x - -
expected     - - - - - - -
```

## Part 2: inactive_camera_among_active

```text
A present camera with no active periods blocks all motion.
threshold = 0.8
camera1 readings = [(1, 0.9), (5, 0.9)]
camera1 declared periods = [(1, 5)]
camera2 readings = [(1, 0.1), (3, 0.2), (5, 0.0)]
camera2 declared periods = []
camera3 readings = [(2, 0.9), (4, 0.9)]
camera3 declared periods = [(2, 4)]
expected = []

time         1 : 2 : 3 : 4 : 5
c1 samples   x - - - - - - - x
c1 periods   x x x x x x x x x
c2 samples   . - - - . - - - .
c2 periods   - - - - - - - - -
c3 samples   - - x - - - x - -
c3 periods   - - x x x x x - -
expected     - - - - - - - - -
```

## Part 2: three_cameras_one_overlap

```text
All three camera conditions must hold.
threshold = 0.8
camera1 readings = [(1, 0.9), (10, 0.9)]
camera1 declared periods = [(1, 10)]
camera2 readings = [(3, 0.9), (8, 0.9)]
camera2 declared periods = [(3, 8)]
camera3 readings = [(5, 0.9), (12, 0.9)]
camera3 declared periods = [(5, 12)]
expected = [(5, 8)]

time         1 : 3 : 5 : 8 : 10 : 12
c1 samples   x - - - - - - -  x -  -
c1 periods   x x x x x x x x  x -  -
c2 samples   - - x - - - x -  - -  -
c2 periods   - - x x x x x -  - -  -
c3 samples   - - - - x - - -  - -  x
c3 periods   - - - - x x x x  x x  x
expected     - - - - x x x -  - -  -
```

## Part 2: three_cameras_only_one_common_instant

```text
The common interval collapses to one endpoint.
threshold = 0.8
camera1 readings = [(1, 0.9), (5, 0.9)]
camera1 declared periods = [(1, 5)]
camera2 readings = [(3, 0.9), (7, 0.9)]
camera2 declared periods = [(3, 7)]
camera3 readings = [(5, 0.9), (9, 0.9)]
camera3 declared periods = [(5, 9)]
expected = [(5, 5)]

time         1 : 3 : 5 : 7 : 9
c1 samples   x - - - x - - - -
c1 periods   x x x x x - - - -
c2 samples   - - x - - - x - -
c2 periods   - - x x x x x - -
c3 samples   - - - - x - - - x
c3 periods   - - - - x x x x x
expected     - - - - x - - - -
```

## Part 2: first_two_overlap_third_disjoint

```text
An overlap between the first two cameras is not enough.
threshold = 0.8
camera1 readings = [(1, 0.9), (5, 0.9)]
camera1 declared periods = [(1, 5)]
camera2 readings = [(3, 0.9), (7, 0.9)]
camera2 declared periods = [(3, 7)]
camera3 readings = [(8, 0.9), (10, 0.9)]
camera3 declared periods = [(8, 10)]
expected = []

time         1 : 3 : 5 : 7 : 8 : 10
c1 samples   x - - - x - - - - -  -
c1 periods   x x x x x - - - - -  -
c2 samples   - - x - - - x - - -  -
c2 periods   - - x x x x x - - -  -
c3 samples   - - - - - - - - x -  x
c3 periods   - - - - - - - - x x  x
expected     - - - - - - - - - -  -
```

## Part 2: every_pair_overlaps_but_not_all_three

```text
Each pair shares an active instant, but no instant belongs to all three.
threshold = 0.8
camera1 readings = [(1, 0.9), (2, 0.9)]
camera1 declared periods = [(1, 2)]
camera2 readings = [(2, 0.9), (3, 0.9)]
camera2 declared periods = [(2, 3)]
camera3 readings = [(1, 0.9), (2, 0.2), (3, 0.9)]
camera3 declared periods = [(1, 1), (3, 3)]
expected = []

time         1 : 2 : 3
c1 samples   x - x - -
c1 periods   x x x - -
c2 samples   - - x - x
c2 periods   - - x x x
c3 samples   x - . - x
c3 periods   x - - - x
expected     - - - - -
```

## Part 2: third_camera_removes_one_of_two_periods

```text
The third camera removes the later overlap entirely.
threshold = 0.8
camera1 readings = [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)]
camera1 declared periods = [(1, 4), (7, 10)]
camera2 readings = [(2, 0.9), (5, 0.9), (6.5, 0.2), (8, 0.9), (11, 0.9)]
camera2 declared periods = [(2, 5), (8, 11)]
camera3 readings = [(3, 0.9), (4, 0.9)]
camera3 declared periods = [(3, 4)]
expected = [(3, 4)]

time         1 : 2 : 3 : 4 : 5 : 5.5 : 6.5 : 7 : 8 : 10 : 11
c1 samples   x - - - - - x - - -   . -   - - x - - -  x -  -
c1 periods   x x x x x x x - - -   - -   - - x x x x  x -  -
c2 samples   - - x - - - - - x -   - -   . - - - x -  - -  x
c2 periods   - - x x x x x x x -   - -   - - - - x x  x x  x
c3 samples   - - - - x - x - - -   - -   - - - - - -  - -  -
c3 periods   - - - - x x x - - -   - -   - - - - - -  - -  -
expected     - - - - x x x - - -   - -   - - - - - -  - -  -
```

## Part 2: third_camera_splits_common_period

```text
A later camera splits one common period into two.
threshold = 0.8
camera1 readings = [(1, 0.9), (12, 0.9)]
camera1 declared periods = [(1, 12)]
camera2 readings = [(2, 0.9), (11, 0.9)]
camera2 declared periods = [(2, 11)]
camera3 readings = [(3, 0.9), (5, 0.9), (6.5, 0.2), (8, 0.9), (10, 0.9)]
camera3 declared periods = [(3, 5), (8, 10)]
expected = [(3, 5), (8, 10)]

time         1 : 2 : 3 : 5 : 6.5 : 8 : 10 : 11 : 12
c1 samples   x - - - - - - -   - - - -  - -  - -  x
c1 periods   x x x x x x x x   x x x x  x x  x x  x
c2 samples   - - x - - - - -   - - - -  - -  x -  -
c2 periods   - - x x x x x x   x x x x  x x  x -  -
c3 samples   - - - - x - x -   . - x -  x -  - -  -
c3 periods   - - - - x x x -   - - x x  x -  - -  -
expected     - - - - x x x -   - - x x  x -  - -  -
```

## Part 2: third_camera_trims_fragmented_chain

```text
The third camera trims existing fragments without filling any camera gaps.
threshold = 0.8
camera1 readings = [(1, 0.9), (10, 0.9)]
camera1 declared periods = [(1, 10)]
camera2 readings = [(0, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (7, 0.9), (7.5, 0.2), (8, 0.9), (12, 0.9)]
camera2 declared periods = [(0, 3), (4, 7), (8, 12)]
camera3 readings = [(2, 0.9), (5, 0.9), (6, 0.2), (7, 0.9), (9, 0.9)]
camera3 declared periods = [(2, 5), (7, 9)]
expected = [(2, 3), (4, 5), (7, 7), (8, 9)]

time         0 : 1 : 2 : 3 : 3.5 : 4 : 5 : 6 : 7 : 7.5 : 8 : 9 : 10 : 12
c1 samples   - - x - - - - -   - - - - - - - - - -   - - - - - -  x -  -
c1 periods   - - x x x x x x   x x x x x x x x x x   x x x x x x  x -  -
c2 samples   x - - - - - x -   . - x - - - - - x -   . - x - - -  - -  x
c2 periods   x x x x x x x -   - - x x x x x x x -   - - x x x x  x x  x
c3 samples   - - - - x - - -   - - - - x - . - x -   - - - - x -  - -  -
c3 periods   - - - - x x x x   x x x x x - - - x x   x x x x x -  - -  -
expected     - - - - x x x -   - - x x x - - - x -   - - x x x -  - -  -
```

## Part 2: three_cameras_fragmented_chain

```text
Staggered positive gaps from all three cameras remain in the output.
threshold = 0.8
camera1 readings = [(1, 0.9), (4, 0.9), (4.5, 0.2), (5, 0.9), (9, 0.9)]
camera1 declared periods = [(1, 4), (5, 9)]
camera2 readings = [(2, 0.9), (6, 0.9), (6.5, 0.2), (7, 0.9), (10, 0.9)]
camera2 declared periods = [(2, 6), (7, 10)]
camera3 readings = [(3, 0.9), (5, 0.9), (5.5, 0.2), (6, 0.9), (8, 0.9)]
camera3 declared periods = [(3, 5), (6, 8)]
expected = [(3, 4), (5, 5), (6, 6), (7, 8)]

time         1 : 2 : 3 : 4 : 4.5 : 5 : 5.5 : 6 : 6.5 : 7 : 8 : 9 : 10
c1 samples   x - - - - - x -   . - x -   - - - -   - - - - - - x -  -
c1 periods   x x x x x x x -   - - x x   x x x x   x x x x x x x -  -
c2 samples   - - x - - - - -   - - - -   - - x -   . - x - - - - -  x
c2 periods   - - x x x x x x   x x x x   x x x -   - - x x x x x x  x
c3 samples   - - - - x - - -   - - x -   . - x -   - - - - x - - -  -
c3 periods   - - - - x x x x   x x x -   - - x x   x x x x x - - -  -
expected     - - - - x x x -   - - x -   - - x -   - - x x x - - -  -
```

## Part 2: four_cameras_nested

```text
More than three cameras, all constraining the result.
threshold = 0.8
camera1 readings = [(0, 0.9), (12, 0.9)]
camera1 declared periods = [(0, 12)]
camera2 readings = [(1, 0.9), (10, 0.9)]
camera2 declared periods = [(1, 10)]
camera3 readings = [(3, 0.9), (9, 0.9)]
camera3 declared periods = [(3, 9)]
camera4 readings = [(5, 0.9), (7, 0.9)]
camera4 declared periods = [(5, 7)]
expected = [(5, 7)]

time         0 : 1 : 3 : 5 : 7 : 9 : 10 : 12
c1 samples   x - - - - - - - - - - -  - -  x
c1 periods   x x x x x x x x x x x x  x x  x
c2 samples   - - x - - - - - - - - -  x -  -
c2 periods   - - x x x x x x x x x x  x -  -
c3 samples   - - - - x - - - - - x -  - -  -
c3 periods   - - - - x x x x x x x -  - -  -
c4 samples   - - - - - - x - x - - -  - -  -
c4 periods   - - - - - - x x x - - -  - -  -
expected     - - - - - - x x x - - -  - -  -
```

## Part 2: fifth_camera_blocks_everything

```text
The last of five cameras removes all remaining common activity.
threshold = 0.8
camera1 readings = [(0, 0.9), (12, 0.9)]
camera1 declared periods = [(0, 12)]
camera2 readings = [(1, 0.9), (10, 0.9)]
camera2 declared periods = [(1, 10)]
camera3 readings = [(3, 0.9), (9, 0.9)]
camera3 declared periods = [(3, 9)]
camera4 readings = [(5, 0.9), (7, 0.9)]
camera4 declared periods = [(5, 7)]
camera5 readings = [(8, 0.9), (11, 0.9)]
camera5 declared periods = [(8, 11)]
expected = []

time         0 : 1 : 3 : 5 : 7 : 8 : 9 : 10 : 11 : 12
c1 samples   x - - - - - - - - - - - - -  - -  - -  x
c1 periods   x x x x x x x x x x x x x x  x x  x x  x
c2 samples   - - x - - - - - - - - - - -  x -  - -  -
c2 periods   - - x x x x x x x x x x x x  x -  - -  -
c3 samples   - - - - x - - - - - - - x -  - -  - -  -
c3 periods   - - - - x x x x x x x x x -  - -  - -  -
c4 samples   - - - - - - x - x - - - - -  - -  - -  -
c4 periods   - - - - - - x x x - - - - -  - -  - -  -
c5 samples   - - - - - - - - - - x - - -  - -  x -  -
c5 periods   - - - - - - - - - - x x x x  x x  x -  -
expected     - - - - - - - - - - - - - -  - -  - -  -
```

## Part 2: duplicate_camera_stream

```text
Repeating one camera does not duplicate output periods.
threshold = 0.8
camera1 readings = [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)]
camera1 declared periods = [(1, 4), (7, 10)]
camera2 readings = [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)]
camera2 declared periods = [(1, 4), (7, 10)]
camera3 readings = [(2, 0.9), (8, 0.9)]
camera3 declared periods = [(2, 8)]
expected = [(2, 4), (7, 8)]

time         1 : 2 : 4 : 5.5 : 7 : 8 : 10
c1 samples   x - - - x -   . - x - - -  x
c1 periods   x x x x x -   - - x x x x  x
c2 samples   x - - - x -   . - x - - -  x
c2 periods   x x x x x -   - - x x x x  x
c3 samples   - - x - - -   - - - - x -  -
c3 periods   - - x x x x   x x x x x -  -
expected     - - x x x -   - - x x x -  -
```

## Part 2: very_sparse_readings

```text
Different sample rates and wide gaps within active runs.
threshold = 0.8
camera1 readings = [(0, 0.9), (10000, 0.9)]
camera1 declared periods = [(0, 10000)]
camera2 readings = [(100, 0.9), (9000, 0.9)]
camera2 declared periods = [(100, 9000)]
camera3 readings = [(200, 0.9), (8000, 0.9)]
camera3 declared periods = [(200, 8000)]
expected = [(200, 8000)]

time         0 : 100 : 200 : 8000 : 9000 : 10000
c1 samples   x -   - -   - -    - -    - -     x
c1 periods   x x   x x   x x    x x    x x     x
c2 samples   - -   x -   - -    - -    x -     -
c2 periods   - -   x x   x x    x x    x -     -
c3 samples   - -   - -   x -    x -    - -     -
c3 periods   - -   - -   x x    x -    - -     -
expected     - -   - -   x x    x -    - -     -
```
