#!/usr/bin/env python3
"""Render literal fixtures as continuous-time coverage diagrams.

Each ':' column represents the open span between neighboring timestamps.
Use --write to generate CASES.md, or filter with --part and --match.
"""

import argparse
from pathlib import Path

from cases import PART1_CASES, PART2_CASES, __doc__ as CASE_PARTITIONS


def draw_case(case, max_ticks=40, max_width=120, include_legend=True):
    streams = case.streams
    timestamps = {time for stream in streams for time, _ in stream}
    declared_periods = list(case.expected)
    if case.camera_periods is not None:
        for periods in case.camera_periods:
            declared_periods.extend(periods)
    timestamps.update(time for period in declared_periods for time in period)
    timestamps = sorted(timestamps)
    if not timestamps:
        return "(empty timeline: no readings; expected [])"

    # The axis includes every declared endpoint, so coverage cannot change
    # inside a span column. No numerical time grid or midpoint is needed.
    columns = []
    for index, timestamp in enumerate(timestamps):
        if index:
            columns.append((timestamps[index - 1], timestamp))
        columns.append((timestamp, timestamp))
    labels = [str(left) if left == right else ":" for left, right in columns]
    widths = [max(2, len(label) + 1) for label in labels]
    label_width = 12
    total_width = label_width + sum(widths)
    if len(timestamps) > max_ticks or total_width > max_width:
        return "SKIPPED: {} timestamps / {} columns; limits {} timestamps / {} columns.".format(
            len(timestamps), total_width, max_ticks, max_width
        )

    def row(label, symbols):
        return label.ljust(label_width) + "".join(
            str(symbol).rjust(width) for symbol, width in zip(symbols, widths)
        )

    def interval_row(label, periods):
        # Rendering supplied interval literals, not finding or intersecting runs.
        return row(
            label,
            [
                "x" if any(start <= left and right <= end for start, end in periods) else "-"
                for left, right in columns
            ],
        )

    lines = [row("time", labels)]
    for number, stream in enumerate(streams, start=1):
        samples = dict(stream)
        lines.append(
            row(
                "c{} samples".format(number),
                [
                    (
                        "-"
                        if left != right or left not in samples
                        else "x" if samples[left] >= case.threshold else "."
                    )
                    for left, right in columns
                ],
            )
        )
        if case.camera_periods is not None:
            lines.append(
                interval_row(
                    "c{} periods".format(number), case.camera_periods[number - 1]
                )
            )
    lines.append(interval_row("expected", case.expected))
    if include_legend:
        lines.append("Each ':' column represents the open span between its neighboring timestamps.")
    return "\n".join(lines)


def describe_case(case, max_ticks=40, max_width=120, include_legend=True):
    lines = [case.reason, "threshold = {}".format(case.threshold)]
    for number, stream in enumerate(case.streams, start=1):
        lines.append("camera{} readings = {}".format(number, stream))
        if case.camera_periods is not None:
            lines.append(
                "camera{} declared periods = {}".format(
                    number, case.camera_periods[number - 1]
                )
            )
    if not case.streams:
        lines.append("camera_streams = []")
    lines.append("expected = {}".format(case.expected))
    lines.extend(["", draw_case(case, max_ticks, max_width, include_legend)])
    return "\n".join(lines)


def render_catalog(selected, max_ticks=40, max_width=120):
    lines = [
        "# Motion test cases",
        "",
        "Fixtures: [cases.py](cases.py). Contract: [README.md](README.md).",
        "",
        "## Case partitions",
        "",
        CASE_PARTITIONS.strip(),
        "",
        "## Diagram legend",
        "",
        "The axis shows sample timestamps and interval endpoints. Each `:` column is the",
        "open span between neighboring timestamps; spacing is schematic.",
        "",
        "- Period rows: `x` = active at that instant or throughout that span; `-` = inactive.",
        "- Sample rows: `x` = at or above threshold; `.` = below threshold; `-` = no reading.",
        "- `SKIPPED` = diagram exceeds the size limits; fixture data remains listed.",
        "",
    ]
    for part, case in selected:
        lines.extend(
            [
                "## Part {}: {}".format(part, case.name),
                "",
                "```text",
                describe_case(case, max_ticks, max_width, include_legend=False),
                "```",
                "",
            ]
        )
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--part", type=int, choices=(1, 2))
    parser.add_argument("--match", default="", help="substring of case name")
    parser.add_argument("--max-ticks", type=int, default=40, help="maximum distinct displayed timestamps")
    parser.add_argument("--max-width", type=int, default=120)
    parser.add_argument("--write", type=Path, help="write the catalog to this path")
    args = parser.parse_args()
    if args.max_ticks < 1 or args.max_width < 1:
        parser.error("diagram limits must be positive")
    selected = [
        (part, case)
        for part, cases in ((1, PART1_CASES), (2, PART2_CASES))
        for case in cases
        if (args.part is None or args.part == part) and args.match in case.name
    ]
    if not selected:
        parser.error("no matching cases")
    output = render_catalog(selected, args.max_ticks, args.max_width)
    if args.write:
        args.write.write_text(output, encoding="utf-8")
        print("Wrote {} cases to {}".format(len(selected), args.write))
    else:
        print(output)


if __name__ == "__main__":
    main()
