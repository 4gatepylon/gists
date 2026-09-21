from pathlib import Path
import subprocess
import sys


FOLDER = Path(__file__).resolve().parent


def test_full_sample_stdin_stdout() -> None:
    result = subprocess.run(
        [sys.executable, str(FOLDER / "main.py")],
        input=(FOLDER / "sample.in").read_text(),
        text=True, capture_output=True, timeout=5,
    )
    assert result.returncode == 0, result.stderr
    assert result.stdout.split() == (FOLDER / "sample.expected").read_text().split()
