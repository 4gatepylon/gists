# Motion periods

Implement `find_active_periods(readings, threshold)` and
`find_common_active_periods(camera_streams, threshold)` in `main.py`.
Both functions receive raw `(timestamp, intensity)` readings.

## Contract

- Timestamps are strictly increasing within each stream and may be fractional.
  Intensities and threshold are in `[0, 1]`.
- Output is a sorted list of `(start, end)` tuples representing maximal closed
  intervals in continuous time. Singletons `(t, t)` are valid.
- Every positive gap remains. Timestamps and threshold comparisons use no
  rounding or tolerance.
- Empty input returns `[]`. Part 2 also returns `[]` if any camera has no active
  periods.

## Part 1: Active periods

Return one period per consecutive run of readings whose intensity is at least
`threshold`, from the first qualifying timestamp to the last. A below-threshold
reading separates runs; sampling gaps alone do not.

```python
readings = [
    (1, 0.4), (5, 0.2), (11, 0.9), (15, 0.9), (17, 0.8),
    (20, 0.3), (27, 0.9), (31, 1.0), (36, 0.8),
]
threshold = 0.8
expected = [(11, 17), (27, 36)]
```

The [Part 1 coverage table](CASES.md#part-1-input-and-output-lengths) groups tests
by input length and output length.

## Part 2: Common active periods

Return maximal periods where every camera is active under Part 1's definition,
using one shared threshold. Camera order is irrelevant. With one camera, return
its Part 1 periods.

| Camera 1 periods | Camera 2 periods | Common periods |
| --- | --- | --- |
| `[(2, 4)]` | `[(1, 2), (4, 5)]` | `[(2, 2), (4, 4)]` |
| `[(2, 4)]` | `[(1, 3), (4, 5)]` | `[(2, 3), (4, 4)]` |
| `[(1, 2)]` | `[(3, 4)]` | `[]` |
| `[(1, 3)]` | `[(3, 5)]` | `[(3, 3)]` |
| `[(0.125, 0.625)]` | `[(0.375, 0.875)]` | `[(0.375, 0.625)]` |

## Tests

From this folder, with Python 3.8+:

```sh
python3 -m pip install -r requirements-dev.txt
python3 -m pytest -q
```

Use `-k part1` or `-k part2` to select a part, and `-s` to show diagnostics on
passing tests. Failures include inputs, expected outputs, returned values, and
diagrams.

[cases.py](cases.py) contains 113 handwritten fixtures: 43 for Part 1 and 70 for
Part 2. Part 2 has separate arrays for zero, one, two, and three-or-more streams,
combined as `PART2_CASES`. Category comments describe the test-space partitions.
Multi-camera cases also check reversed and rotated camera orders.

## Diagrams

[CASES.md](CASES.md) lists every fixture and its coverage diagram, with a shared
legend. Regenerate it after changing fixtures:

```sh
python3 visualize_cases.py --write CASES.md
python3 visualize_cases.py --part 2 --match overlaps
```

`--max-ticks` and `--max-width` limit diagrams to 40 timestamps and 120 columns
by default. Larger diagrams show `SKIPPED`; their fixture data remains visible.
