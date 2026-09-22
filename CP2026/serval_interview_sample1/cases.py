"""### Part 1: input and output lengths

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
"""

from typing import List, NamedTuple, Optional, Tuple, Union

Timestamp = Union[int, float]
Reading = Tuple[Timestamp, float]
Period = Tuple[Timestamp, Timestamp]


class Case(NamedTuple):
    name: str
    # Raw readings passed to the functions.
    streams: List[List[Reading]]
    expected: List[Period]
    reason: str
    threshold: float
    # Declared Part 1 periods for diagnostics and diagrams.
    camera_periods: Optional[List[List[Period]]]


PART1_CASES = [
    ### Empty input and activity cardinality ###
    Case(
        name='empty_stream',
        streams=[[]],
        expected=[],
        reason='No observations.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='single_below',
        streams=[[(5, 0.2)]],
        expected=[],
        reason='One observation, strictly below.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='single_equal',
        streams=[[(5, 0.8)]],
        expected=[(5, 5)],
        reason='One observation, exactly equal.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='single_above',
        streams=[[(5, 0.9)]],
        expected=[(5, 5)],
        reason='One observation, strictly above.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='never_active',
        streams=[[(1, 0.0), (3, 0.2), (8, 0.79)]],
        expected=[],
        reason='Several readings, none active.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='always_strictly_above',
        streams=[[(1, 0.9), (2, 1.0), (6, 0.85)]],
        expected=[(1, 6)],
        reason='An all-active stream ends at its final reading.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='always_exactly_equal',
        streams=[[(2, 0.8), (5, 0.8), (9, 0.8)]],
        expected=[(2, 9)],
        reason='Equality is active throughout.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='always_mixed_equal_above',
        streams=[[(1, 0.8), (4, 0.95), (6, 0.8), (7, 1.0)]],
        expected=[(1, 7)],
        reason='Equality and strict activity share one run.',
        threshold=0.8, camera_periods=None,
    ),

    ### Run position, length, and equality at boundaries ###
    Case(
        name='singleton_at_start',
        streams=[[(1, 0.9), (2, 0.1), (3, 0.2)]],
        expected=[(1, 1)],
        reason='Only the first index is active.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='singleton_in_middle',
        streams=[[(1, 0.2), (2, 0.9), (3, 0.2)]],
        expected=[(2, 2)],
        reason='Only an interior index is active.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='singleton_at_end',
        streams=[[(1, 0.2), (2, 0.1), (3, 0.9)]],
        expected=[(3, 3)],
        reason='Only the final index is active.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='equal_singleton_at_start',
        streams=[[(1, 0.8), (2, 0.2), (3, 0.2)]],
        expected=[(1, 1)],
        reason='Exactly-threshold singleton at the start.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='equal_singleton_in_middle',
        streams=[[(1, 0.2), (2, 0.8), (3, 0.2)]],
        expected=[(2, 2)],
        reason='Exactly-threshold singleton in the middle.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='equal_singleton_at_end',
        streams=[[(1, 0.2), (2, 0.2), (3, 0.8)]],
        expected=[(3, 3)],
        reason='Exactly-threshold singleton at the end.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='multi_run_at_start',
        streams=[[(1, 0.9), (3, 0.95), (7, 0.2)]],
        expected=[(1, 3)],
        reason='Two active readings followed by an inactive one.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='multi_run_in_middle',
        streams=[[(1, 0.2), (3, 0.9), (5, 1.0), (7, 0.2)]],
        expected=[(3, 5)],
        reason='An active run bracketed by inactive readings.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='multi_run_at_end',
        streams=[[(1, 0.2), (3, 0.9), (5, 1.0)]],
        expected=[(3, 5)],
        reason='An active run reaches the end of input.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='equal_multi_run_in_middle',
        streams=[[(1, 0.2), (3, 0.8), (5, 0.8), (7, 0.2)]],
        expected=[(3, 5)],
        reason='A multi-reading run can consist entirely of equality.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='equal_edges_strict_interior',
        streams=[[(1, 0.2), (2, 0.8), (4, 0.95), (6, 0.8), (7, 0.2)]],
        expected=[(2, 6)],
        reason='Equality supplies both endpoints.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='equal_bridge_between_above',
        streams=[[(1, 0.9), (3, 0.8), (5, 0.9)]],
        expected=[(1, 5)],
        reason='An exactly-threshold reading does not split a run.',
        threshold=0.8, camera_periods=None,
    ),

    ### Inactive readings and transitions between runs ###
    Case(
        name='one_below_splits_runs',
        streams=[[(1, 0.9), (2, 0.8), (3, 0.79), (4, 0.8), (5, 0.9)]],
        expected=[(1, 2), (4, 5)],
        reason='One below-threshold observation really splits activity.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='alternating_starts_active',
        streams=[[(1, 0.9), (2, 0.2), (3, 0.8), (4, 0.2), (5, 1.0)]],
        expected=[(1, 1), (3, 3), (5, 5)],
        reason='Alternating readings with both edges active.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='alternating_starts_inactive',
        streams=[[(1, 0.2), (2, 0.8), (3, 0.2), (4, 0.9), (5, 0.2)]],
        expected=[(2, 2), (4, 4)],
        reason='Alternating readings with both edges inactive.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='mixed_run_lengths',
        streams=[[(1, 0.9), (2, 0.2), (3, 0.8), (4, 0.9), (5, 1.0), (6, 0.2), (7, 0.2), (8, 0.9), (9, 0.2), (10, 0.8), (12, 0.9)]],
        expected=[(1, 1), (3, 5), (8, 8), (10, 12)],
        reason='Singletons, longer runs, and different inactive-gap lengths.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='many_inactive_between_runs',
        streams=[[(1, 0.9), (2, 0.9), (3, 0.1), (4, 0.2), (5, 0.0), (6, 0.9), (7, 0.9)]],
        expected=[(1, 2), (6, 7)],
        reason='Several consecutive inactive readings create one separation.',
        threshold=0.8, camera_periods=None,
    ),

    ### Run endpoints, sparse sampling, and timestamp scale ###
    Case(
        name='last_active_not_first_inactive',
        streams=[[(2, 0.9), (5, 0.9), (100, 0.2)]],
        expected=[(2, 5)],
        reason='The period ends at 5, not the next reading at 100.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='first_active_not_previous_inactive',
        streams=[[(1, 0.2), (100, 0.9), (101, 0.9)]],
        expected=[(100, 101)],
        reason='The period starts at 100, not the previous reading.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='sparse_active_readings_stay_one_run',
        streams=[[(1, 0.9), (100, 0.8), (10000, 0.9)]],
        expected=[(1, 10000)],
        reason='No maximum allowed gap between adjacent active readings.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='sparse_inactive_reading_splits',
        streams=[[(1, 0.9), (100, 0.2), (10000, 0.9)]],
        expected=[(1, 1), (10000, 10000)],
        reason='Sparse timestamps do not erase a below-threshold reading.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='timestamp_zero',
        streams=[[(0, 0.8), (1, 0.9), (2, 0.2)]],
        expected=[(0, 1)],
        reason='Timestamp zero is a valid start.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='large_timestamps',
        streams=[[(1000000000000, 0.8), (1000000000007, 0.9), (1000000000100, 0.2)]],
        expected=[(1000000000000, 1000000000007)],
        reason='Return timestamp values, not list indices.',
        threshold=0.8, camera_periods=None,
    ),

    ### Threshold extremes, custom values, and exact comparisons ###
    Case(
        name='threshold_zero',
        streams=[[(1, 0.0), (4, 0.3), (8, 1.0)]],
        expected=[(1, 8)],
        reason='At threshold zero, even zero intensity is active.',
        threshold=0.0, camera_periods=None,
    ),
    Case(
        name='threshold_one_none',
        streams=[[(1, 0.0), (2, 0.9), (3, 0.999999)]],
        expected=[],
        reason='Below one is inactive when threshold is one.',
        threshold=1.0, camera_periods=None,
    ),
    Case(
        name='threshold_one_all',
        streams=[[(1, 1.0), (2, 1.0), (5, 1.0)]],
        expected=[(1, 5)],
        reason='Equality at the highest valid threshold.',
        threshold=1.0, camera_periods=None,
    ),
    Case(
        name='threshold_one_mixed',
        streams=[[(1, 1.0), (2, 0.99), (3, 1.0), (4, 1.0), (5, 0.0)]],
        expected=[(1, 1), (3, 4)],
        reason='Only exact ones qualify at threshold one.',
        threshold=1.0, camera_periods=None,
    ),
    Case(
        name='custom_threshold',
        streams=[[(1, 0.49), (2, 0.5), (3, 0.51), (4, 0.49)]],
        expected=[(2, 3)],
        reason='The threshold is an argument, not a hardcoded 0.8.',
        threshold=0.5, camera_periods=None,
    ),
    Case(
        name='close_to_threshold',
        streams=[[(1, 0.799999), (2, 0.8), (3, 0.800001), (4, 0.799999)]],
        expected=[(2, 3)],
        reason='Close values on both sides; no tolerance is specified.',
        threshold=0.8, camera_periods=None,
    ),

    ### Continuous time: fractional endpoints and positive gaps ###
    Case(
        name='adjacent_runs_preserved_in_part1',
        streams=[[(1, 0.9), (3, 0.8), (3.5, 0.2), (4, 0.9), (6, 0.8)]],
        expected=[(1, 3), (4, 6)],
        reason='The inactive reading at 3.5 creates a positive gap, preserved in both parts.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='adjacent_singletons_preserved_in_part1',
        streams=[[(2, 0.8), (2.5, 0.2), (3, 0.9)]],
        expected=[(2, 2), (3, 3)],
        reason='An inactive reading at 2.5 separates two singleton periods in continuous time.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='fractional_active_endpoints',
        streams=[[(0.125, 0.8), (0.5, 0.9), (0.75, 0.2)]],
        expected=[(0.125, 0.5)],
        reason='Qualifying timestamps may be fractional; preserve the exact endpoints.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='fractional_gap_between_runs',
        streams=[[(1, 0.9), (1.25, 0.8), (1.3125, 0.2), (1.375, 0.9), (1.5, 0.8)]],
        expected=[(1, 1.25), (1.375, 1.5)],
        reason='A positive gap smaller than one time unit still separates runs.',
        threshold=0.8, camera_periods=None,
    ),
    Case(
        name='tiny_positive_gap_between_runs',
        streams=[[(0, 0.9), (1, 0.9), (1.00000000005, 0.2), (1.0000000001, 0.8), (2, 0.9)]],
        expected=[(0, 1), (1.0000000001, 2)],
        reason='Even a very small positive gap is retained; there is no timestamp tolerance.',
        threshold=0.8, camera_periods=None,
    ),

    ### Example with two active runs ###
    Case(
        name='interview_example',
        streams=[[(1, 0.4), (5, 0.2), (11, 0.9), (15, 0.9), (17, 0.8), (20, 0.3), (27, 0.9), (31, 1.0), (36, 0.8)]],
        expected=[(11, 17), (27, 36)],
        reason='Two active runs separated by a below-threshold reading.',
        threshold=0.8, camera_periods=None,
    ),
]


PART2_NO_STREAM_CASES = [
    ### Zero streams: defined empty-input behavior ###
    Case(
        name='no_cameras',
        streams=[
        ],
        expected=[],
        reason='The defined empty-camera result is [].',
        threshold=0.8,
        camera_periods=[],
    ),
]


PART2_ONE_STREAM_CASES = [
    ### Empty stream and one active run ###
    Case(
        name='one_empty_camera',
        streams=[
            [],
        ],
        expected=[],
        reason='One camera with no observations.',
        threshold=0.8,
        camera_periods=[[]],
    ),
    Case(
        name='one_camera_one_run',
        streams=[
            [(2, 0.9), (6, 0.9)],
        ],
        expected=[(2, 6)],
        reason='One camera is sufficient; no second camera is required.',
        threshold=0.8,
        camera_periods=[[(2, 6)]],
    ),

    ### Multiple runs: preserve every positive gap ###
    Case(
        name='one_camera_separated_runs',
        streams=[
            [(1, 0.9), (2, 0.9), (3.5, 0.2), (5, 0.9), (7, 0.9)],
        ],
        expected=[(1, 2), (5, 7)],
        reason='One camera preserves separated periods.',
        threshold=0.8,
        camera_periods=[[(1, 2), (5, 7)]],
    ),
    Case(
        name='one_camera_adjacent_runs',
        streams=[
            [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (6, 0.9)],
        ],
        expected=[(1, 3), (4, 6)],
        reason='One camera preserves the entire gap (3,4), including the inactive reading at 3.5.',
        threshold=0.8,
        camera_periods=[[(1, 3), (4, 6)]],
    ),
]


PART2_TWO_STREAM_CASES = [
    ### Inactive cameras block common activity ###
    Case(
        name='all_cameras_inactive',
        streams=[
            [(1, 0.1), (4, 0.2)],
            [(2, 0.0), (5, 0.7)],
        ],
        expected=[],
        reason='Both cameras have readings, but none qualify.',
        threshold=0.8,
        camera_periods=[[], []],
    ),
    Case(
        name='one_camera_just_below_threshold',
        streams=[
            [(1, 0.9), (5, 0.9)],
            [(1, 0.799999), (5, 0.799999)],
        ],
        expected=[],
        reason='A nearly-active camera still blocks the entire result.',
        threshold=0.8,
        camera_periods=[[(1, 5)], []],
    ),

    ### One-period geometry: equality, overlap, and containment ###
    Case(
        name='identical_periods',
        streams=[
            [(2, 0.9), (6, 0.9)],
            [(2, 0.9), (6, 0.9)],
        ],
        expected=[(2, 6)],
        reason='Identical starts and ends.',
        threshold=0.8,
        camera_periods=[[(2, 6)], [(2, 6)]],
    ),
    Case(
        name='partial_overlap',
        streams=[
            [(1, 0.9), (5, 0.9)],
            [(3, 0.9), (8, 0.9)],
        ],
        expected=[(3, 5)],
        reason='Starts and ends are staggered.',
        threshold=0.8,
        camera_periods=[[(1, 5)], [(3, 8)]],
    ),
    Case(
        name='partial_overlap_reversed',
        streams=[
            [(3, 0.9), (8, 0.9)],
            [(1, 0.9), (5, 0.9)],
        ],
        expected=[(3, 5)],
        reason='The same geometry in the opposite camera order.',
        threshold=0.8,
        camera_periods=[[(3, 8)], [(1, 5)]],
    ),
    Case(
        name='first_contains_second',
        streams=[
            [(1, 0.9), (10, 0.9)],
            [(3, 0.9), (7, 0.9)],
        ],
        expected=[(3, 7)],
        reason='Strict containment.',
        threshold=0.8,
        camera_periods=[[(1, 10)], [(3, 7)]],
    ),
    Case(
        name='second_contains_first',
        streams=[
            [(3, 0.9), (7, 0.9)],
            [(1, 0.9), (10, 0.9)],
        ],
        expected=[(3, 7)],
        reason='Containment with the shorter camera first.',
        threshold=0.8,
        camera_periods=[[(3, 7)], [(1, 10)]],
    ),
    Case(
        name='same_start_different_ends',
        streams=[
            [(2, 0.9), (5, 0.9)],
            [(2, 0.9), (8, 0.9)],
        ],
        expected=[(2, 5)],
        reason='Equal starts do not imply equal ends.',
        threshold=0.8,
        camera_periods=[[(2, 5)], [(2, 8)]],
    ),
    Case(
        name='same_end_different_starts',
        streams=[
            [(1, 0.9), (7, 0.9)],
            [(4, 0.9), (7, 0.9)],
        ],
        expected=[(4, 7)],
        reason='Equal ends do not imply equal starts.',
        threshold=0.8,
        camera_periods=[[(1, 7)], [(4, 7)]],
    ),

    ### Disjoint periods and distinct singleton times ###
    Case(
        name='disjoint_first_earlier',
        streams=[
            [(1, 0.9), (3, 0.9)],
            [(6, 0.9), (8, 0.9)],
        ],
        expected=[],
        reason='First camera finishes before the second starts.',
        threshold=0.8,
        camera_periods=[[(1, 3)], [(6, 8)]],
    ),
    Case(
        name='disjoint_second_earlier',
        streams=[
            [(6, 0.9), (8, 0.9)],
            [(1, 0.9), (3, 0.9)],
        ],
        expected=[],
        reason='Second camera finishes before the first starts.',
        threshold=0.8,
        camera_periods=[[(6, 8)], [(1, 3)]],
    ),
    Case(
        name='adjacent_cameras_do_not_overlap',
        streams=[
            [(1, 0.9), (2, 0.9)],
            [(3, 0.9), (4, 0.9)],
        ],
        expected=[],
        reason='The positive gap (2,3) means the cameras never overlap.',
        threshold=0.8,
        camera_periods=[[(1, 2)], [(3, 4)]],
    ),
    Case(
        name='interleaved_but_disjoint',
        streams=[
            [(1, 0.9), (2, 0.9), (4.5, 0.2), (7, 0.9), (8, 0.9)],
            [(4, 0.9), (5, 0.9), (7.5, 0.2), (10, 0.9), (11, 0.9)],
        ],
        expected=[],
        reason='Multiple alternating periods, but no shared active time.',
        threshold=0.8,
        camera_periods=[[(1, 2), (7, 8)], [(4, 5), (10, 11)]],
    ),
    Case(
        name='different_singletons',
        streams=[
            [(3, 0.9)],
            [(4, 0.9)],
        ],
        expected=[],
        reason='Consecutive singletons in different cameras are still disjoint.',
        threshold=0.8,
        camera_periods=[[(3, 3)], [(4, 4)]],
    ),

    ### Closed endpoints and singleton intersections ###
    Case(
        name='endpoint_only_overlap',
        streams=[
            [(1, 0.9), (4, 0.9)],
            [(4, 0.9), (7, 0.9)],
        ],
        expected=[(4, 4)],
        reason='A shared endpoint is a valid singleton overlap.',
        threshold=0.8,
        camera_periods=[[(1, 4)], [(4, 7)]],
    ),
    Case(
        name='same_singleton',
        streams=[
            [(3, 0.9)],
            [(3, 0.9)],
        ],
        expected=[(3, 3)],
        reason='Two cameras active at the same single instant.',
        threshold=0.8,
        camera_periods=[[(3, 3)], [(3, 3)]],
    ),
    Case(
        name='singleton_inside_period',
        streams=[
            [(4, 0.9)],
            [(1, 0.9), (7, 0.9)],
        ],
        expected=[(4, 4)],
        reason='A singleton strictly inside a longer period.',
        threshold=0.8,
        camera_periods=[[(4, 4)], [(1, 7)]],
    ),
    Case(
        name='singleton_at_period_start',
        streams=[
            [(2, 0.9)],
            [(2, 0.9), (7, 0.9)],
        ],
        expected=[(2, 2)],
        reason="Singleton at the other period's start.",
        threshold=0.8,
        camera_periods=[[(2, 2)], [(2, 7)]],
    ),
    Case(
        name='singleton_at_period_end',
        streams=[
            [(7, 0.9)],
            [(2, 0.9), (7, 0.9)],
        ],
        expected=[(7, 7)],
        reason="Singleton at the other period's end.",
        threshold=0.8,
        camera_periods=[[(7, 7)], [(2, 7)]],
    ),

    ### Multiple overlaps and positive gaps on one segment ###
    Case(
        name='one_overlap_on_one_segment',
        streams=[
            [(2, 0.9), (4, 0.9)],
            [(1, 0.9), (2, 0.9), (4, 0.2), (6, 0.9), (7, 0.9)],
        ],
        expected=[(2, 2)],
        reason="Only one of the second camera's periods overlaps.",
        threshold=0.8,
        camera_periods=[[(2, 4)], [(1, 2), (6, 7)]],
    ),
    Case(
        name='two_endpoint_overlaps_on_one_segment',
        streams=[
            [(2, 0.9), (4, 0.9)],
            [(1, 0.9), (2, 0.9), (3, 0.2), (4, 0.9), (5, 0.9)],
        ],
        expected=[(2, 2), (4, 4)],
        reason='The positive gap (2,4) separates the two singleton outputs.',
        threshold=0.8,
        camera_periods=[[(2, 4)], [(1, 2), (4, 5)]],
    ),
    Case(
        name='two_wide_overlaps_on_one_segment',
        streams=[
            [(2, 0.9), (10, 0.9)],
            [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (12, 0.9)],
        ],
        expected=[(2, 4), (7, 10)],
        reason='Two positive-length overlaps remain separated.',
        threshold=0.8,
        camera_periods=[[(2, 10)], [(1, 4), (7, 12)]],
    ),
    Case(
        name='two_adjacent_overlaps_preserve_gap',
        streams=[
            [(2, 0.9), (4, 0.9)],
            [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (5, 0.9)],
        ],
        expected=[(2, 3), (4, 4)],
        reason='The positive gap (3,4) must remain even though its endpoints are consecutive integers.',
        threshold=0.8,
        camera_periods=[[(2, 4)], [(1, 3), (4, 5)]],
    ),
    Case(
        name='two_adjacent_singleton_overlaps_preserve_gap',
        streams=[
            [(2, 0.9), (3, 0.9)],
            [(1, 0.9), (2, 0.9), (2.5, 0.2), (3, 0.9), (4, 0.9)],
        ],
        expected=[(2, 2), (3, 3)],
        reason='Distinct singleton overlaps do not include any time between them.',
        threshold=0.8,
        camera_periods=[[(2, 3)], [(1, 2), (3, 4)]],
    ),
    Case(
        name='inactive_reading_prevents_merge',
        streams=[
            [(2, 0.9), (5, 0.9)],
            [(1, 0.9), (2, 0.9), (3, 0.2), (4, 0.9), (6, 0.9)],
        ],
        expected=[(2, 2), (4, 5)],
        reason='The inactive reading at 3 separates runs; preserve the whole gap (2,4).',
        threshold=0.8,
        camera_periods=[[(2, 5)], [(1, 2), (4, 6)]],
    ),
    Case(
        name='three_overlaps_on_one_segment',
        streams=[
            [(1, 0.9), (12, 0.9)],
            [(0, 0.9), (2, 0.9), (3.5, 0.2), (5, 0.9), (7, 0.9), (8.5, 0.2), (10, 0.9), (14, 0.9)],
        ],
        expected=[(1, 2), (5, 7), (10, 12)],
        reason='One long run is reused for three separated overlaps.',
        threshold=0.8,
        camera_periods=[[(1, 12)], [(0, 2), (5, 7), (10, 14)]],
    ),
    Case(
        name='three_adjacent_overlaps_preserve_gaps',
        streams=[
            [(1, 0.9), (9, 0.9)],
            [(0, 0.9), (2, 0.9), (2.5, 0.2), (3, 0.9), (5, 0.9), (5.5, 0.2), (6, 0.9), (10, 0.9)],
        ],
        expected=[(1, 2), (3, 5), (6, 9)],
        reason='Both positive gaps in a chain of three overlap pieces remain.',
        threshold=0.8,
        camera_periods=[[(1, 9)], [(0, 2), (3, 5), (6, 10)]],
    ),
    Case(
        name='small_gaps_around_larger_gap',
        streams=[
            [(2, 0.9), (12, 0.9)],
            [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (5, 0.9), (6.5, 0.2), (8, 0.9), (9, 0.9), (9.5, 0.2), (10, 0.9), (14, 0.9)],
        ],
        expected=[(2, 3), (4, 5), (8, 9), (10, 12)],
        reason='Preserve the smaller gaps (3,4) and (9,10), as well as the larger gap (5,8).',
        threshold=0.8,
        camera_periods=[[(2, 12)], [(1, 3), (4, 5), (8, 9), (10, 14)]],
    ),

    ### Fragmentation across both streams and changing interval boundaries ###
    Case(
        name='later_chain_after_earlier_chain',
        streams=[
            [(2, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)],
            [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (5, 0.9), (5.5, 0.2), (6, 0.9), (8, 0.9), (8.5, 0.2), (9, 0.9), (11, 0.9)],
        ],
        expected=[(2, 3), (4, 4), (7, 8), (9, 10)],
        reason='Both long periods have multiple overlaps whose positive gaps remain.',
        threshold=0.8,
        camera_periods=[[(2, 4), (7, 10)], [(1, 3), (4, 5), (6, 8), (9, 11)]],
    ),
    Case(
        name='both_cameras_fragmented_chains',
        streams=[
            [(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (6, 0.9), (7.5, 0.2), (9, 0.9), (11, 0.9), (11.5, 0.2), (12, 0.9), (15, 0.9)],
            [(2, 0.9), (4, 0.9), (4.5, 0.2), (5, 0.9), (7, 0.9), (8.5, 0.2), (10, 0.9), (12, 0.9), (12.5, 0.2), (13, 0.9), (14, 0.9)],
        ],
        expected=[(2, 3), (4, 4), (5, 6), (10, 11), (12, 12), (13, 14)],
        reason='Gaps from either camera remain, including around singleton overlaps.',
        threshold=0.8,
        camera_periods=[[(1, 3), (4, 6), (9, 11), (12, 15)], [(2, 4), (5, 7), (10, 12), (13, 14)]],
    ),
    Case(
        name='alternating_which_camera_ends_first',
        streams=[
            [(1, 0.9), (5, 0.9), (6.5, 0.2), (8, 0.9), (12, 0.9), (13.5, 0.2), (15, 0.9), (19, 0.9)],
            [(3, 0.9), (9, 0.9), (10, 0.2), (11, 0.9), (16, 0.9), (17, 0.2), (18, 0.9), (22, 0.9)],
        ],
        expected=[(3, 5), (8, 9), (11, 12), (15, 16), (18, 19)],
        reason='Several overlaps; the earlier ending camera changes repeatedly.',
        threshold=0.8,
        camera_periods=[[(1, 5), (8, 12), (15, 19)], [(3, 9), (11, 16), (18, 22)]],
    ),
    Case(
        name='early_nonmatches_then_overlap',
        streams=[
            [(1, 0.9), (2, 0.9), (3.5, 0.2), (5, 0.9), (6, 0.9), (7.5, 0.2), (9, 0.9), (12, 0.9)],
            [(10, 0.9), (14, 0.9)],
        ],
        expected=[(10, 12)],
        reason='Several early periods do not intersect anything.',
        threshold=0.8,
        camera_periods=[[(1, 2), (5, 6), (9, 12)], [(10, 14)]],
    ),
    Case(
        name='overlap_then_trailing_nonmatches',
        streams=[
            [(1, 0.9), (4, 0.9), (6, 0.2), (8, 0.9), (9, 0.9), (10.5, 0.2), (12, 0.9), (13, 0.9)],
            [(2, 0.9), (3, 0.9)],
        ],
        expected=[(2, 3)],
        reason='Only the first period intersects; later ones must not leak into output.',
        threshold=0.8,
        camera_periods=[[(1, 4), (8, 9), (12, 13)], [(2, 3)]],
    ),
    Case(
        name='equal_ends_then_later_overlap',
        streams=[
            [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)],
            [(2, 0.9), (4, 0.9), (6, 0.2), (8, 0.9), (12, 0.9)],
        ],
        expected=[(2, 4), (8, 10)],
        reason='Equal first endpoints followed by more work.',
        threshold=0.8,
        camera_periods=[[(1, 4), (7, 10)], [(2, 4), (8, 12)]],
    ),
    Case(
        name='repeated_endpoint_only_overlaps',
        streams=[
            [(2, 0.9), (4, 0.9), (6, 0.2), (8, 0.9), (10, 0.9)],
            [(1, 0.9), (2, 0.9), (3, 0.2), (4, 0.9), (8, 0.9), (9, 0.2), (10, 0.9), (11, 0.9)],
        ],
        expected=[(2, 2), (4, 4), (8, 8), (10, 10)],
        reason='Four endpoint-only overlaps across two long periods.',
        threshold=0.8,
        camera_periods=[[(2, 4), (8, 10)], [(1, 2), (4, 8), (10, 11)]],
    ),
    Case(
        name='staggered_gaps_form_larger_gap',
        streams=[
            [(0, 0.9), (2, 0.9), (3, 0.2), (4, 0.9), (8, 0.9)],
            [(0, 0.9), (3, 0.9), (4, 0.2), (5, 0.9), (8, 0.9)],
        ],
        expected=[(0, 2), (5, 8)],
        reason='The output gap (2,5) combines gaps from different cameras; neither alone spans it.',
        threshold=0.8,
        camera_periods=[[(0, 2), (4, 8)], [(0, 3), (5, 8)]],
    ),

    ### Sampling and timestamp scale ###
    Case(
        name='overlap_without_shared_reading_timestamps',
        streams=[
            [(1, 0.9), (6, 0.9)],
            [(4, 0.9), (10, 0.9)],
        ],
        expected=[(4, 6)],
        reason='No raw timestamp is shared; their inferred periods still overlap.',
        threshold=0.8,
        camera_periods=[[(1, 6)], [(4, 10)]],
    ),
    Case(
        name='timestamp_zero_overlap',
        streams=[
            [(0, 0.9), (3, 0.9)],
            [(0, 0.9)],
        ],
        expected=[(0, 0)],
        reason='Zero is a valid singleton overlap.',
        threshold=0.8,
        camera_periods=[[(0, 3)], [(0, 0)]],
    ),
    Case(
        name='large_timestamps_overlap',
        streams=[
            [(1000000000000, 0.9), (1000000000010, 0.9)],
            [(1000000000004, 0.9), (1000000000020, 0.9)],
        ],
        expected=[(1000000000004, 1000000000010)],
        reason='Large timestamp values must be preserved exactly.',
        threshold=0.8,
        camera_periods=[[(1000000000000, 1000000000010)], [(1000000000004, 1000000000020)]],
    ),

    ### Threshold equality, extremes, and custom values ###
    Case(
        name='threshold_equality_at_overlap_boundaries',
        streams=[
            [(0, 0.2), (1, 0.9), (4, 0.8), (6, 0.2)],
            [(1, 0.2), (2, 0.8), (5, 0.9), (7, 0.2)],
        ],
        expected=[(2, 4)],
        reason='Both overlap boundaries are exactly threshold in their own camera.',
        threshold=0.8,
        camera_periods=[[(1, 4)], [(2, 5)]],
    ),
    Case(
        name='exact_threshold_singletons',
        streams=[
            [(1, 0.2), (3, 0.8), (5, 0.2)],
            [(2, 0.2), (3, 0.8), (4, 0.2)],
        ],
        expected=[(3, 3)],
        reason='Equality alone supplies a shared singleton.',
        threshold=0.8,
        camera_periods=[[(3, 3)], [(3, 3)]],
    ),
    Case(
        name='threshold_zero_different_extents',
        streams=[
            [(1, 0.0), (5, 0.2)],
            [(3, 0.0), (8, 1.0)],
        ],
        expected=[(3, 5)],
        reason='All readings qualify, but camera time extents still constrain overlap.',
        threshold=0.0,
        camera_periods=[[(1, 5)], [(3, 8)]],
    ),
    Case(
        name='threshold_one_split_overlap',
        streams=[
            [(1, 1.0), (2, 1.0), (3, 0.99), (4, 1.0), (5, 1.0)],
            [(1, 0.9), (2, 1.0), (4, 1.0), (5, 0.9)],
        ],
        expected=[(2, 2), (4, 4)],
        reason='Only readings equal to one qualify, including isolated boundary overlaps.',
        threshold=1.0,
        camera_periods=[[(1, 2), (4, 5)], [(2, 4)]],
    ),
    Case(
        name='custom_threshold_all_cameras',
        streams=[
            [(1, 0.49), (2, 0.5), (4, 0.6), (6, 0.49)],
            [(2, 0.49), (3, 0.5), (5, 0.9), (7, 0.49)],
        ],
        expected=[(3, 4)],
        reason='A nondefault threshold applies to every stream.',
        threshold=0.5,
        camera_periods=[[(2, 4)], [(3, 5)]],
    ),

    ### Continuous time: fractional overlap, endpoints, and gaps ###
    Case(
        name='fractional_overlap_without_integer_timestamp',
        streams=[
            [(0.125, 0.9), (0.625, 0.9)],
            [(0.375, 0.8), (0.875, 0.9)],
        ],
        expected=[(0.375, 0.625)],
        reason='A nonempty common interval can contain no integer timestamp.',
        threshold=0.8,
        camera_periods=[[(0.125, 0.625)], [(0.375, 0.875)]],
    ),
    Case(
        name='fractional_shared_endpoint',
        streams=[
            [(0.125, 0.9), (0.375, 0.9)],
            [(0.375, 0.8), (0.625, 0.9)],
        ],
        expected=[(0.375, 0.375)],
        reason='Closed intervals meeting at a fractional endpoint share exactly that instant.',
        threshold=0.8,
        camera_periods=[[(0.125, 0.375)], [(0.375, 0.625)]],
    ),
    Case(
        name='fractional_disjoint_cameras',
        streams=[
            [(0.125, 0.9), (0.375, 0.9)],
            [(0.5, 0.9), (0.875, 0.9)],
        ],
        expected=[],
        reason='A sub-unit gap between cameras is still disjoint; timestamps are not rounded.',
        threshold=0.8,
        camera_periods=[[(0.125, 0.375)], [(0.5, 0.875)]],
    ),
    Case(
        name='fractional_gap_preserved',
        streams=[
            [(0, 0.9), (2, 0.9)],
            [(1, 0.9), (1.25, 0.8), (1.3125, 0.2), (1.375, 0.9), (1.5, 0.8)],
        ],
        expected=[(1, 1.25), (1.375, 1.5)],
        reason='Activity in the first camera cannot fill the second camera\'s fractional gap.',
        threshold=0.8,
        camera_periods=[[(0, 2)], [(1, 1.25), (1.375, 1.5)]],
    ),
    Case(
        name='tiny_positive_gap_preserved',
        streams=[
            [(0, 0.9), (3, 0.9)],
            [(0, 0.9), (1, 0.9), (1.00000000005, 0.2), (1.0000000001, 0.8), (2, 0.9)],
        ],
        expected=[(0, 1), (1.0000000001, 2)],
        reason='A very small positive gap must not be filled by a tolerance or adjacency rule.',
        threshold=0.8,
        camera_periods=[[(0, 3)], [(0, 1), (1.0000000001, 2)]],
    ),
    Case(
        name='uninterrupted_fractional_period',
        streams=[
            [(0, 0.9), (0.25, 0.8), (0.75, 0.9), (1, 0.9)],
            [(0.125, 0.9), (0.5, 0.8), (0.875, 0.9)],
        ],
        expected=[(0.125, 0.875)],
        reason='Interior sample boundaries introduce no gap or redundant output split.',
        threshold=0.8,
        camera_periods=[[(0, 1)], [(0.125, 0.875)]],
    ),
]


PART2_THREE_OR_MORE_STREAM_CASES = [
    ### Empty or inactive cameras block all common activity ###
    Case(
        name='all_cameras_empty',
        streams=[
            [],
            [],
            [],
        ],
        expected=[],
        reason='Several cameras, all empty.',
        threshold=0.8,
        camera_periods=[[], [], []],
    ),
    Case(
        name='empty_camera_among_active',
        streams=[
            [(1, 0.9), (5, 0.9)],
            [],
            [(2, 0.9), (4, 0.9)],
        ],
        expected=[],
        reason='Every camera is required, including an empty one.',
        threshold=0.8,
        camera_periods=[[(1, 5)], [], [(2, 4)]],
    ),
    Case(
        name='inactive_camera_among_active',
        streams=[
            [(1, 0.9), (5, 0.9)],
            [(1, 0.1), (3, 0.2), (5, 0.0)],
            [(2, 0.9), (4, 0.9)],
        ],
        expected=[],
        reason='A present camera with no active periods blocks all motion.',
        threshold=0.8,
        camera_periods=[[(1, 5)], [], [(2, 4)]],
    ),

    ### All-camera agreement versus pairwise overlap ###
    Case(
        name='three_cameras_one_overlap',
        streams=[
            [(1, 0.9), (10, 0.9)],
            [(3, 0.9), (8, 0.9)],
            [(5, 0.9), (12, 0.9)],
        ],
        expected=[(5, 8)],
        reason='All three camera conditions must hold.',
        threshold=0.8,
        camera_periods=[[(1, 10)], [(3, 8)], [(5, 12)]],
    ),
    Case(
        name='three_cameras_only_one_common_instant',
        streams=[
            [(1, 0.9), (5, 0.9)],
            [(3, 0.9), (7, 0.9)],
            [(5, 0.9), (9, 0.9)],
        ],
        expected=[(5, 5)],
        reason='The common interval collapses to one endpoint.',
        threshold=0.8,
        camera_periods=[[(1, 5)], [(3, 7)], [(5, 9)]],
    ),
    Case(
        name='first_two_overlap_third_disjoint',
        streams=[
            [(1, 0.9), (5, 0.9)],
            [(3, 0.9), (7, 0.9)],
            [(8, 0.9), (10, 0.9)],
        ],
        expected=[],
        reason='An overlap between the first two cameras is not enough.',
        threshold=0.8,
        camera_periods=[[(1, 5)], [(3, 7)], [(8, 10)]],
    ),
    Case(
        name='every_pair_overlaps_but_not_all_three',
        streams=[
            [(1, 0.9), (2, 0.9)],
            [(2, 0.9), (3, 0.9)],
            [(1, 0.9), (2, 0.2), (3, 0.9)],
        ],
        expected=[],
        reason='Each pair shares an active instant, but no instant belongs to all three.',
        threshold=0.8,
        camera_periods=[[(1, 2)], [(2, 3)], [(1, 1), (3, 3)]],
    ),

    ### Later cameras remove, trim, or split common periods ###
    Case(
        name='third_camera_removes_one_of_two_periods',
        streams=[
            [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)],
            [(2, 0.9), (5, 0.9), (6.5, 0.2), (8, 0.9), (11, 0.9)],
            [(3, 0.9), (4, 0.9)],
        ],
        expected=[(3, 4)],
        reason='The third camera removes the later overlap entirely.',
        threshold=0.8,
        camera_periods=[[(1, 4), (7, 10)], [(2, 5), (8, 11)], [(3, 4)]],
    ),
    Case(
        name='third_camera_splits_common_period',
        streams=[
            [(1, 0.9), (12, 0.9)],
            [(2, 0.9), (11, 0.9)],
            [(3, 0.9), (5, 0.9), (6.5, 0.2), (8, 0.9), (10, 0.9)],
        ],
        expected=[(3, 5), (8, 10)],
        reason='A later camera splits one common period into two.',
        threshold=0.8,
        camera_periods=[[(1, 12)], [(2, 11)], [(3, 5), (8, 10)]],
    ),
    Case(
        name='third_camera_trims_fragmented_chain',
        streams=[
            [(1, 0.9), (10, 0.9)],
            [(0, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (7, 0.9), (7.5, 0.2), (8, 0.9), (12, 0.9)],
            [(2, 0.9), (5, 0.9), (6, 0.2), (7, 0.9), (9, 0.9)],
        ],
        expected=[(2, 3), (4, 5), (7, 7), (8, 9)],
        reason='The third camera trims existing fragments without filling any camera gaps.',
        threshold=0.8,
        camera_periods=[[(1, 10)], [(0, 3), (4, 7), (8, 12)], [(2, 5), (7, 9)]],
    ),
    Case(
        name='three_cameras_fragmented_chain',
        streams=[
            [(1, 0.9), (4, 0.9), (4.5, 0.2), (5, 0.9), (9, 0.9)],
            [(2, 0.9), (6, 0.9), (6.5, 0.2), (7, 0.9), (10, 0.9)],
            [(3, 0.9), (5, 0.9), (5.5, 0.2), (6, 0.9), (8, 0.9)],
        ],
        expected=[(3, 4), (5, 5), (6, 6), (7, 8)],
        reason='Staggered positive gaps from all three cameras remain in the output.',
        threshold=0.8,
        camera_periods=[[(1, 4), (5, 9)], [(2, 6), (7, 10)], [(3, 5), (6, 8)]],
    ),

    ### Four or five cameras and duplicate streams ###
    Case(
        name='four_cameras_nested',
        streams=[
            [(0, 0.9), (12, 0.9)],
            [(1, 0.9), (10, 0.9)],
            [(3, 0.9), (9, 0.9)],
            [(5, 0.9), (7, 0.9)],
        ],
        expected=[(5, 7)],
        reason='More than three cameras, all constraining the result.',
        threshold=0.8,
        camera_periods=[[(0, 12)], [(1, 10)], [(3, 9)], [(5, 7)]],
    ),
    Case(
        name='fifth_camera_blocks_everything',
        streams=[
            [(0, 0.9), (12, 0.9)],
            [(1, 0.9), (10, 0.9)],
            [(3, 0.9), (9, 0.9)],
            [(5, 0.9), (7, 0.9)],
            [(8, 0.9), (11, 0.9)],
        ],
        expected=[],
        reason='The last of five cameras removes all remaining common activity.',
        threshold=0.8,
        camera_periods=[[(0, 12)], [(1, 10)], [(3, 9)], [(5, 7)], [(8, 11)]],
    ),
    Case(
        name='duplicate_camera_stream',
        streams=[
            [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)],
            [(1, 0.9), (4, 0.9), (5.5, 0.2), (7, 0.9), (10, 0.9)],
            [(2, 0.9), (8, 0.9)],
        ],
        expected=[(2, 4), (7, 8)],
        reason='Repeating one camera does not duplicate output periods.',
        threshold=0.8,
        camera_periods=[[(1, 4), (7, 10)], [(1, 4), (7, 10)], [(2, 8)]],
    ),

    ### Sparse sampling across multiple cameras ###
    Case(
        name='very_sparse_readings',
        streams=[
            [(0, 0.9), (10000, 0.9)],
            [(100, 0.9), (9000, 0.9)],
            [(200, 0.9), (8000, 0.9)],
        ],
        expected=[(200, 8000)],
        reason='Different sample rates and wide gaps within active runs.',
        threshold=0.8,
        camera_periods=[[(0, 10000)], [(100, 9000)], [(200, 8000)]],
    ),
]


# Combined view retained for the test runner and diagram renderer.
PART2_CASES = (
    PART2_NO_STREAM_CASES
    + PART2_ONE_STREAM_CASES
    + PART2_TWO_STREAM_CASES
    + PART2_THREE_OR_MORE_STREAM_CASES
)
