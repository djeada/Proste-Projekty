import importlib.util
import re
import time
from pathlib import Path

import pytest

SRC = Path(__file__).resolve().parent.parent / "src"


def load(name):
    spec = importlib.util.spec_from_file_location(name, SRC / f"{name}.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


@pytest.fixture(autouse=True)
def no_sleep(monkeypatch):
    monkeypatch.setattr(time, "sleep", lambda seconds: None)


@pytest.mark.parametrize(
    "name, expected",
    [
        ("donut", "3 frames rendered. no GPU needed."),
        ("mandelbrot_zoom", "100 iterations"),
        ("game_of_life", "generation   2"),
        ("matrix_rain", "Follow the white rabbit."),
        ("doom_fire", "fire extinguished"),
        ("spinning_cube", "\033[38;5;196m█"),
        ("plasma", "3 frames of plasma, zero textures"),
        ("ray_tracer", "3 frames ray traced on the CPU"),
        ("warp_starfield", "arrived at Alpha Centauri"),
    ],
)
def test_effect_draws_frames(name, expected, capsys):
    load(name).main(3)
    out = capsys.readouterr().out
    assert out.startswith(("\033[2J", "\033[1;32m[*]"))
    assert out.count("\033[H") >= 3
    assert expected in out


def test_quicksort_sorts_and_finishes_green(monkeypatch, capsys):
    quicksort = load("quicksort_visualizer")
    monkeypatch.setattr(quicksort, "N", 12)
    quicksort.main()
    frames = capsys.readouterr().out.split("\033[H")
    assert "quicksort | compares" in frames[1]
    rows = frames[-1].splitlines()[2:]
    assert len(rows) == quicksort.H
    for row in rows:  # sorted bars grow to the right and are all green
        assert re.fullmatch(" *(\033\\[38;5;46m█+)?\033\\[0m", row)
    assert "█" * 12 in rows[-1]


def test_maze_is_solved(monkeypatch, capsys):
    maze = load("maze_solver")
    monkeypatch.setattr(maze, "MW", 15)
    monkeypatch.setattr(maze, "MH", 9)
    maze.main()
    out = capsys.readouterr().out
    assert out.count("\033[H") > 10
    assert "█" in out
    assert "shortest path:" in out
