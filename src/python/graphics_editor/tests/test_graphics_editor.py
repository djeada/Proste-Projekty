"""Tests of the paint logic: no window is needed."""
from graphics_editor import (BLACK, HISTORY_DEPTH, WHITE, Canvas, History, draw_line, draw_rect,
                             flood_fill)

RED = (255, 0, 0)
BLUE = (0, 0, 255)


def count(canvas, color):
    return sum(1 for pixel in canvas.pixels if pixel == color)


def test_new_canvas_is_white():
    canvas = Canvas(8, 6)
    assert len(canvas.pixels) == 48
    assert count(canvas, WHITE) == 48


def test_set_and_get_ignore_borders():
    canvas = Canvas(4, 4)
    canvas.set(1, 2, RED)
    assert canvas.get(1, 2) == RED
    canvas.set(-1, 0, RED)
    canvas.set(4, 0, RED)
    assert count(canvas, RED) == 1
    assert canvas.get(4, 0) == BLACK


def test_horizontal_line_endpoints_and_pixel_count():
    canvas = Canvas(10, 10)
    draw_line(canvas, 2, 5, 7, 5, 1, RED)
    assert canvas.get(2, 5) == RED
    assert canvas.get(7, 5) == RED
    assert count(canvas, RED) == 6


def test_diagonal_line_and_its_reverse_match():
    forward = Canvas(10, 10)
    backward = Canvas(10, 10)
    draw_line(forward, 0, 0, 4, 4, 1, RED)
    draw_line(backward, 4, 4, 0, 0, 1, RED)
    assert count(forward, RED) == 5
    assert forward.pixels == backward.pixels


def test_steep_line_has_one_pixel_per_row():
    canvas = Canvas(10, 10)
    draw_line(canvas, 1, 0, 3, 9, 1, RED)
    assert count(canvas, RED) == 10
    for y in range(10):
        assert sum(1 for x in range(10) if canvas.get(x, y) == RED) == 1


def test_thick_brush_is_clipped_at_border():
    canvas = Canvas(5, 5)
    draw_line(canvas, 0, 0, 0, 0, 3, RED)
    assert count(canvas, RED) == 4
    draw_line(canvas, 0, 4, 0, 4, 5, BLUE)
    assert count(canvas, BLUE) == 9


def test_rectangle_outline():
    canvas = Canvas(10, 10)
    draw_rect(canvas, 2, 2, 5, 4, 1, RED)
    # A 4 x 3 rectangle has 2 * (4 + 3) - 4 = 10 border pixels.
    assert count(canvas, RED) == 10
    assert canvas.get(3, 3) == WHITE
    assert canvas.get(5, 4) == RED


def test_flood_fill_stops_at_border():
    canvas = Canvas(7, 7)
    draw_rect(canvas, 1, 1, 5, 5, 1, RED)
    flood_fill(canvas, 3, 3, BLUE)
    assert count(canvas, BLUE) == 9
    assert count(canvas, RED) == 16
    assert canvas.get(0, 0) == WHITE


def test_flood_fill_same_color_does_nothing():
    canvas = Canvas(4, 4)
    flood_fill(canvas, 1, 1, WHITE)
    assert count(canvas, WHITE) == 16
    flood_fill(canvas, 9, 9, RED)
    assert count(canvas, RED) == 0


def test_flood_fill_whole_big_canvas():
    canvas = Canvas(320, 240)
    flood_fill(canvas, 0, 0, RED)
    assert count(canvas, RED) == 320 * 240


def test_undo_restores_previous_image():
    history = History()
    canvas = Canvas(4, 4)
    history.push(canvas)
    canvas.set(0, 0, RED)
    restored = history.undo()
    assert restored is not None
    assert restored.get(0, 0) == WHITE
    assert history.undo() is None


def test_undo_history_drops_oldest():
    history = History()
    canvas = Canvas(2, 2)
    for _ in range(HISTORY_DEPTH + 5):
        canvas.set(0, 0, RED)
        history.push(canvas)
    undone = 0
    while history.undo() is not None:
        undone += 1
    assert undone == HISTORY_DEPTH


def test_copy_is_independent():
    original = Canvas(3, 3)
    original.set(1, 1, RED)
    clone = original.copy()
    original.set(0, 0, BLUE)
    assert clone.get(1, 1) == RED
    assert clone.get(0, 0) == WHITE
