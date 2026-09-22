#!/usr/bin/env python3
"""Test motion periods against the literal fixtures in cases.py.

Contract: closed intervals in continuous time; every positive gap remains.
Fixtures are grouped by stream count and test-space category in cases.py.
CASES.md contains inputs, expected outputs, and diagrams.

Each invocation receives fresh input copies. Multi-camera cases also check
reversed and rotated orders. Part 2 prints separate Part 1 diagnostics for each
camera; these neither trace Part 2 nor add assertions. Failure diagrams show
fixture data, while runtime printouts label actual results separately.

Run: python3 -m pytest -q
Filter with -k part1 or -k part2; use -s to show passing diagnostics.
"""

from copy import deepcopy

import pytest

from cases import PART1_CASES, PART2_CASES
from main import find_active_periods, find_common_active_periods
from visualize_cases import describe_case


def check_result(function, argument, case, order="original camera order"):
    part = "Part 1" if case.camera_periods is None else "Part 2"
    print("\n[{}] {} | {}".format(case.name, part, order))
    print("EXPECTED {} periods (fixture): {!r}".format(part, case.expected))
    detail = "\n{} ({})\nFIXTURE DATA AND EXPECTED DIAGRAM (not your output):\n{}".format(
        case.name, order, describe_case(case)
    )
    try:
        actual = function(deepcopy(argument), case.threshold)
    except NotImplementedError as error:
        print("YOUR {} raised {}: {}".format(part, type(error).__name__, error))
        pytest.fail(str(error) + detail, pytrace=False)
    except Exception as error:
        # Pytest displays captured stdout on failure, alongside the traceback.
        print("YOUR {} raised {}: {}".format(part, type(error).__name__, error))
        print(detail)
        raise
    print("YOUR {} returned periods: {!r}".format(part, actual))
    assert actual == case.expected, detail


def print_part1_diagnostics(case):
    print("\n[{}] YOUR PART 1: separate diagnostic calls per camera".format(case.name))
    print("Camera numbers below match the original fixture/diagram order.")
    print("threshold = {!r}".format(case.threshold))
    if not case.streams:
        print("No cameras; no Part 1 calls.")
    for number, (stream, expected) in enumerate(
        zip(case.streams, case.camera_periods), start=1
    ):
        print("\ncamera{} readings: {!r}".format(number, stream))
        print("  EXPECTED Part 1 periods (fixture): {!r}".format(expected))
        try:
            actual = find_active_periods(deepcopy(stream), case.threshold)
        except Exception as error:
            # Diagnostics must not prevent testing an independent Part 2.
            print("  YOUR Part 1 raised {}: {}".format(type(error).__name__, error))
        else:
            print("  YOUR Part 1 returned periods: {!r}".format(actual))


@pytest.mark.parametrize("case", PART1_CASES, ids=lambda case: case.name)
def test_part1(case):
    check_result(find_active_periods, case.streams[0], case)


@pytest.mark.parametrize("case", PART2_CASES, ids=lambda case: case.name)
def test_part2(case):
    print_part1_diagnostics(case)
    check_result(find_common_active_periods, case.streams, case)
    if len(case.streams) > 1:
        seen = [case.streams]
        variants = [
            ("reversed cameras", list(reversed(case.streams))),
            ("rotated cameras", case.streams[1:] + case.streams[:1]),
        ]
        for label, streams in variants:
            if streams not in seen:
                seen.append(streams)
                check_result(find_common_active_periods, streams, case, label)
