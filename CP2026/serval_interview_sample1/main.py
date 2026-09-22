"""Implement these two functions, then run: python3 -m pytest -q

See README.md for the inclusive integer-time contract and empty-input behavior.

TODO(hadriano) verify continuous time semantics more clearly/carefully
"""

import itertools
from typing import List, Tuple, Union

Timestamp = Union[int, float]
Reading = Tuple[Timestamp, float]
Period = Tuple[int, int]


# O(N) where N = len(readings)
def find_active_periods(readings: List[Reading], threshold: float) -> List[Period]:
    """Return each consecutive active run's first and last reading timestamps.

    A reading is active when intensity >= threshold. Preserve distinct runs,
    even if their integer endpoints are adjacent. Empty readings produce [].
    """
    intervals_inclusive = []
    if len(readings) == 0:
        return intervals_inclusive

    previous_i = None
    final_timestamp, _ = readings[-1]
    readings.append((final_timestamp + 1, threshold - 1))  # Sentinel
    for i, (timestamp, intensity) in enumerate(readings):
        is_collecting = previous_i is not None
        is_active = intensity >= threshold
        assert (
            not is_collecting
        ) or previous_i < i, f"Previous i={previous_i} >= i={i}"
        if is_collecting and not is_active:  # Close
            # Sanity check and extract values from indices
            assert i > 0, f"i={i} is 0"
            previous_timestamp, _ = readings[previous_i]
            immediately_before_timestamp, _ = readings[i - 1]

            # Sanity-check and extract/insert interval
            assert (
                previous_timestamp <= immediately_before_timestamp
            ), f"previous_timestamp={previous_timestamp} > immediately_before_timestamp={immediately_before_timestamp}"
            intervals_inclusive.append(
                (previous_timestamp, immediately_before_timestamp)
            )
            previous_i = None
        elif not is_collecting and is_active:  # Open
            previous_i = i
    assert (
        previous_i is None
    ), f"previous_i={previous_i} is not None"  # Either closed before sentinel or closed BY Sentinel
    readings.pop()  # Remove sentinel

    # Sanity check/validate output and reutnr
    assert all(0 <= i[0] <= i[1] for i in intervals_inclusive)
    assert all(
        i1[1] < i2[0] for i1, i2 in zip(intervals_inclusive, intervals_inclusive[1:])
    )
    return intervals_inclusive


def _disjoint_after(interval1: Period, interval2: Period) -> bool:
    """Return if interval1 and interval2 are disjoint AND interval2 starts AFTER interval1"""
    _, end1 = interval1
    start2, _ = interval2
    return end1 < start2


def _overlap(interval1: Period, interval2: Period) -> bool:
    start1, end1 = interval1
    start2, end2 = interval2
    return start1 <= end2 and end1 >= start2


def _union(interval1: Period, interval2: Period) -> Period:
    start1, end1 = interval1
    start2, end2 = interval2
    start, end = min(start1, start2), max(end1, end2)
    assert (
        start <= end
    ), f"start={start} > end={end} from _union(interval1={interval1}, interval2={interval2})"
    return (start, end)


def _intersection(interval1: Period, interval2: Period) -> Period:
    start1, end1 = interval1
    start2, end2 = interval2
    start, end = max(start1, start2), min(end1, end2)
    assert (
        start <= end
    ), f"start={start} > end={end} from _intersection(interval1={interval1}, interval2={interval2})"
    return (start, end)


# NOTE: if you ended up with [a, b] and [b+1, c] it appened either BECAUSE exactly:
#   You had [x, b] and [b+1, y] for x <= a, y <= c in ONE stream (with the a from the other).
#   This MUST have happened beause of an INTERSECTION operation being used here (those entries need to have been previously in BOTH).
#   It's possible at least ONE stream had b in a larger interval, but they cannot BOTH have had that.
#   This means that these cases should NOT be merged into longer intgervals since if this split existed, then
#   the previous camera stream active periods would have already been merged. More broadly, you should never merge after the fact (ANY
#   seperation IS MEANINGFUL and should be KEPT).
#
# In the first case the two entries should NOT be merged since otherwise they would have been merged in the stream itself.
# In the second case you
#
# TODO(hadriano) how to prove this? I am not sure. I think it might be some combination of like:
# - if there is a gap in the output, was there a gap somewhere in the input?
# - yes because in the 2 case if there were no gap in the input there wouldn't be a gap in the output
# - in the N case all you need is that gaps don't go away; as you can see from ^ gaps don't go away, so they can only accum..
#
# def _canonicalize_intervals(intervals: List[Period]) -> List[Period]:
#     """Return a list of intervals that are canonicalized, i.e. no two adjacent intervals are overlapping"""
#     post_merged_intervals = []
#     for interval in intervals:
#         if len(post_merged_intervals) == 0:
#             post_merged_intervals.append(interval)
#         else:
#             assert not _overlap(post_merged_intervals[-1], interval)
#             if post_merged_intervals[-1][1] + 1 == interval[0]:
#                 post_merged_intervals[-1] = _union(post_merged_intervals[-1], interval)
#             else:
#                 post_merged_intervals.append(interval)
#     assert all(0 <= i[0] <= i[1] for i in post_merged_intervals)
#     assert all(
#         i1[1] < i2[0]
#         for i1, i2 in zip(post_merged_intervals, post_merged_intervals[1:])
#     )
#     return post_merged_intervals


def _intersection_merge_intervals(
    intervals1: List[Period], intervals2: List[Period]
) -> List[Period]:

    # Pre-merged is a set of intervals such that all the intervals IN the pre-merge are
    # correct, BUT they may be split up into adjacent intervals.
    #
    # The algorirthm to produce it is simple: go through all possible overlapping intervals
    # and just take their intersection. This is done in the O(N) way that works like this:
    # ```
    # for each interval1 in intervals1:
    #   find the next interval2 that is EITHER overlapping OR disjoint and AFTER interval1
    #   if overlapping, add their overlap to the pre-merged intervals
    #   else (disjoint and after): move on to the next interval1
    # ```
    pre_merged_intervals, i, j = [], 0, 0
    while i < len(intervals1) and j < len(intervals2):
        # print("> i", i, "j", j, "intervals1[i]", intervals1[i], "intervals2[j]", intervals2[j]) # DEBUG
        # 1. Produce intersection if needed
        if _overlap(intervals1[i], intervals2[j]):
            pre_merge_interval = _intersection(intervals1[i], intervals2[j])
            pre_merged_intervals.append(pre_merge_interval)
        # 2. Move on to the next interval. The correct one to move is the one that has the first
        # end (if you move the one that has the last end, then you are necessarily now disjoint and could
        # miss some overlaps, so that's definately wrong; however you MUST move at least one pointer)
        j_has_first_end = intervals2[j][1] < intervals1[i][1]
        # print("> j_has_first_end", j_has_first_end, "; intervals1[i]", intervals1[i], "intervals2[j]", intervals2[j]) # DEBUG
        if j_has_first_end:
            j += 1
        else:
            i += 1
        # print("> next i,j:", i, ",", j, "; len(intervals1): ", len(intervals1), "; len(intervals2): ", len(intervals2)) # DEBUG

    # Make sure the format is right and return (assertions)
    post_merged_intervals = pre_merged_intervals
    return post_merged_intervals


# O(sum(len(stream) for stream in camera_streams)) i.e. for C streams of N readings each, O(CN)
def find_common_active_periods(
    camera_streams: List[List[Reading]], threshold: float
) -> List[Period]:
    """Return maximal inclusive integer periods active in every camera.

    Merge adjacent output periods: [(2, 3), (4, 4)] becomes [(2, 4)].
    No cameras, or any camera with no active periods, produces [].
    """
    if len(camera_streams) == 0:
        return []
    final_intervals = find_active_periods(camera_streams[0], threshold)
    for stream in camera_streams[1:]:
        intervals = find_active_periods(stream, threshold)
        final_intervals = _intersection_merge_intervals(final_intervals, intervals)
    return final_intervals


if __name__ == "__main__":
    # Example camera data
    camera1_readings = [(1, 1.0), (2, 1.0), (3, 0.99), (4, 1.0), (5, 1.0)]
    camera2_readings = [(1, 0.9), (2, 1.0), (4, 1.0), (5, 0.9)]
    threshold = 1.0

    print("Camera 1 readings:", camera1_readings)
    print("Camera 2 readings:", camera2_readings)
    print("Threshold:", threshold)

    periods1 = find_active_periods(camera1_readings, threshold)
    periods2 = find_active_periods(camera2_readings, threshold)
    print("Camera 1 active periods (Part 1):", periods1)
    print("Camera 2 active periods (Part 1):", periods2)
    print("=" * 100)
    for camera1_interval, camera2_interval in itertools.product(periods1, periods2):
        print(
            f"Overlap (part 2) of {camera1_interval} and {camera2_interval}: "
            + f"{_overlap(camera1_interval, camera2_interval)} with intersection "
            + f"{_intersection(camera1_interval, camera2_interval)}"
        )

    camera_streams = [camera1_readings, camera2_readings]
    periods_common = find_common_active_periods(camera_streams, threshold)
    print("=" * 100)
    print("Common active periods (Part 2):", periods_common)
    reversed_camera_streams = list(reversed(camera_streams))
    periods_common_reversed = find_common_active_periods(
        reversed_camera_streams, threshold
    )
    print("Common active periods (Part 2) reversed:", periods_common_reversed)
