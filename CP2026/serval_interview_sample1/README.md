# Serval interview sample 1: motion periods

Source: the interview account supplied in this conversation. No external source
URL, official test suite, or input-size limits were provided. The requested
practice language for this problem is Python.

Implement `find_active_periods` and `find_common_active_periods` in `main.py`.
Both are empty stubs. The tests contain handwritten expected answers, with no
reference solver or solution hints.

## Part 1: Find active motion periods

A camera supplies a list of `(timestamp, intensity)` readings ordered by
timestamp. Each intensity is between 0 and 1. Given a threshold, return the
start and end timestamps of every consecutive run of readings with intensity
greater than or equal to the threshold.

A run starts at its first qualifying reading and ends at its last qualifying
reading. A below-threshold reading separates runs. A gap in sampling alone
does not separate them. A single qualifying reading produces `(t, t)`.

```python
readings = [
    (1, 0.4),
    (5, 0.2),
    (11, 0.9),
    (15, 0.9),
    (17, 0.8),
    (20, 0.3),
    (27, 0.9),
    (31, 1.0),
    (36, 0.8),
]
threshold = 0.8
expected = [(11, 17), (27, 36)]
```

## Part 2: Find periods active in every camera

The input is `camera_streams = [camera1_readings, camera2_readings, ...]` and
one shared threshold. Return periods where **every** camera is active, using
the periods defined in Part 1. Camera order has no significance.

This practice suite follows the requested **inclusive integer-index** convention:
the output describes integer ticks, and consecutive covered ticks belong to
one maximal output period. In particular:

| Camera 1 periods | Camera 2 periods | Expected common periods |
| --- | --- | --- |
| `[(2, 4)]` | `[(1, 2), (4, 5)]` | `[(2, 2), (4, 4)]` |
| `[(2, 4)]` | `[(1, 3), (4, 5)]` | `[(2, 4)]` |
| `[(1, 2)]` | `[(3, 4)]` | `[]` |

In the second row, `[2,3]` and `[4,4]` combine because no integer tick is
missing. In the first row, tick 3 is missing. In the third row, the cameras
never overlap; adjacency between different cameras does not count as overlap.

The tests pass **raw reading lists** to both functions. To express Camera 2's
separate `[1,3]` and `[4,5]` runs in the second example, its raw input is:

```python
[(1, 0.9), (3, 0.9), (3.5, 0.2), (4, 0.9), (5, 0.9)]
```

The inactive half-tick separates the raw runs in Part 1. Part 2 describes only
integer coverage, so that half-tick does not create a missing integer. This is
an explicit local convention to support the requested adjacent-merge examples;
it differs from continuous-time interval semantics.

## Input and output contract

- Readings are lists of `(timestamp, intensity)` tuples, with strictly increasing
  timestamps per camera. Intensities and threshold are in `[0, 1]`.
- For these fixtures, qualifying sample timestamps are nonnegative integers.
  Inactive sample timestamps may also use half ticks such as `3.5`.
- Return a list of `(start, end)` tuples in ascending order. Both endpoints are
  inclusive. Singleton periods are retained.
- Part 1 preserves distinct runs separated by any inactive sample. Part 2 returns
  maximal integer periods, including when only one camera is supplied.
- An empty reading list returns `[]`. Part 2 returns `[]` for no cameras, an
  empty camera, or a camera with no active periods.
- No minimum sampling frequency, gap timeout, intensity interpolation, approximate
  threshold comparison, duplicate-timestamp policy, or input-validation behavior
  is specified. The tests do not impose any of those requirements.
- No official input-size or timestamp bounds were supplied. Local cases include
  timestamps up to approximately `10**12`; there is no performance benchmark.

## Run the tests

From this folder, using Python 3.8 or later:

```sh
python3 -m pip install -r requirements-dev.txt
python3 -m pytest -q
python3 -m pytest -q -k part1
python3 -m pytest -q -k part2
python3 -m pytest -v -k adjacent
python3 -m pytest -x
python3 -m pytest -s -v -k threshold_one_split_overlap
```

From the repository root:

```sh
python3 -m pytest -q CP2026/serval_interview_sample1
```

There are **103 named cases: 40 for Part 1 and 63 for Part 2**. Multi-camera
cases also check distinct reversed and rotated camera orders against the same
answer. Each invocation gets a fresh copy of the inputs. Initially the cases
fail with `NotImplementedError` until you fill in the two stubs.

Runtime diagnostics label **YOUR Part 1 returned periods** and **EXPECTED Part 1
periods (fixture)** explicitly. Part 2 tests also call your Part 1 separately for
each camera, printing its actual return value or exception alongside the expected
periods. These diagnostic calls use fresh input copies, follow the original
camera order, and do not assert Part 1 correctness or trace calls inside Part 2.
Your final Part 2 output is printed separately as **YOUR Part 2 returned periods**.
Pytest shows captured output on failures; add `-s` to see prints on passing runs
as well. The ASCII diagrams themselves continue to show expected fixture data.

`test_motion.py` has the detailed test-space partitions in its module docstring.
Part 1 varies activity, equality, run length/location, transitions, thresholds,
and timestamp spacing. Part 2 varies camera count, overlap geometry, isolated
versus adjacent overlap pieces, repeated chains, all-camera participation,
sampling, thresholds, and camera order. Expected results other than the supplied
interview example are authored practice fixtures, not official interview tests.

## Review the diagrams

Open [CASES.md](CASES.md) to review every input, expected output, and diagram.
The diagrams are generated from the explicit fixture data in `cases.py`, rather
than manually drawn. The renderer does not calculate expected answers.

- `x` means an active integer tick in a period row, or an at-or-above-threshold observation
  in a sample row.
- `-` means no coverage in a period row, or no observation in a sample row.
- `.` means a below-threshold observation in a sample row.
- Fractional inactive observations are listed below the integer diagram.

Long diagrams print `SKIPPED`; their inputs and expected outputs are still shown.
The default limits are 40 integer ticks and 120 columns, checked **before** any
timeline is expanded.

```sh
python3 visualize_cases.py --part 2 --match overlaps
python3 visualize_cases.py --part 2 --match very_sparse
python3 visualize_cases.py --max-ticks 20
python3 visualize_cases.py --write CASES.md
```

Regenerate `CASES.md` after changing fixtures. Failure messages also include the
diagram, so inspecting a failed case does not require opening another file.
