import os
import subprocess
import sys
from pathlib import Path

import pytest

SRC = Path(__file__).resolve().parent.parent / "src"
EFFECTS = sorted(p.stem for p in SRC.glob("*.py") if p.stem != "term")
sys.path.insert(0, str(SRC))


def run(name, frames):
    env = dict(os.environ, NO_SLEEP="1")
    result = subprocess.run(
        [sys.executable, str(SRC / f"{name}.py"), str(frames)],
        capture_output=True, text=True, encoding="utf-8", env=env, timeout=120,
    )
    assert result.returncode == 0, result.stderr
    return result.stdout


@pytest.mark.parametrize("name", EFFECTS)
def test_effect_draws_frames_and_restores_terminal(name):
    out = run(name, 20)
    assert out.startswith("\033[?25l\033[2J")
    assert out.count("\033[H") == 20
    assert out.endswith("\033[0m\033[?25h\n")


def test_random_numbers_match_c_and_javascript():
    import term

    term.seed(1)
    assert [term.rnd(100) for _ in range(5)] == [38, 26, 13, 83, 19]


def test_quicksort_sorts(monkeypatch):
    import quicksort_visualizer as qs

    monkeypatch.setattr(qs, "show_pixels", lambda status: None)
    a = [5, 3, 44, 1, 3, 20, 7]
    qs.Sorter(a).quicksort(0, len(a) - 1)
    assert a == sorted([5, 3, 44, 1, 3, 20, 7])


def test_maze_is_carved_and_solved(monkeypatch):
    import maze_solver as ms

    monkeypatch.setattr(ms, "show_pixels", lambda status: None)
    maze = [[ms.WALL] * ms.MW for _ in range(ms.MH)]
    ms.carve(maze)
    rooms = [maze[y][x] for y in range(1, ms.MH, 2) for x in range(1, ms.MW, 2)]
    assert all(v == ms.OPEN for v in rooms)
    ms.solve(maze)
    assert maze[ms.MH - 2][ms.MW - 2] >= ms.MW + ms.MH - 6
