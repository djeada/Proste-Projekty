/* Tests of the paint logic. Returns 0 when every check passes. */
#include <assert.h>
#include <stdio.h>

#include "graphics_editor.h"

static const Color WHITE = {255, 255, 255};
static const Color RED = {255, 0, 0};
static const Color BLUE = {0, 0, 255};

static int count_color(const Canvas *canvas, Color color)
{
    int count = 0;
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            if (color_equal(canvas_get(canvas, x, y), color)) {
                count++;
            }
        }
    }
    return count;
}

static void test_new_canvas_is_white(void)
{
    Canvas canvas = canvas_create(8, 6, WHITE);
    assert(canvas.width == 8 && canvas.height == 6);
    assert(count_color(&canvas, WHITE) == 48);
    canvas_destroy(&canvas);
}

static void test_set_and_get_ignore_borders(void)
{
    Canvas canvas = canvas_create(4, 4, WHITE);
    canvas_set(&canvas, 1, 2, RED);
    assert(color_equal(canvas_get(&canvas, 1, 2), RED));
    canvas_set(&canvas, -1, 0, RED);
    canvas_set(&canvas, 4, 0, RED);
    assert(count_color(&canvas, RED) == 1);
    assert(!canvas_inside(&canvas, 4, 0));
    canvas_destroy(&canvas);
}

static void test_horizontal_line(void)
{
    Canvas canvas = canvas_create(10, 10, WHITE);
    draw_line(&canvas, 2, 5, 7, 5, 1, RED);
    assert(color_equal(canvas_get(&canvas, 2, 5), RED));
    assert(color_equal(canvas_get(&canvas, 7, 5), RED));
    assert(count_color(&canvas, RED) == 6);
    canvas_destroy(&canvas);
}

static void test_diagonal_line_and_reverse(void)
{
    Canvas a = canvas_create(10, 10, WHITE);
    Canvas b = canvas_create(10, 10, WHITE);
    draw_line(&a, 0, 0, 4, 4, 1, RED);
    draw_line(&b, 4, 4, 0, 0, 1, RED);
    assert(count_color(&a, RED) == 5);
    for (int i = 0; i < 5; i++) {
        assert(color_equal(canvas_get(&a, i, i), RED));
    }
    assert(count_color(&b, RED) == 5);
    assert(count_color(&a, RED) == count_color(&b, RED));
    canvas_destroy(&a);
    canvas_destroy(&b);
}

static void test_steep_line_has_one_pixel_per_row(void)
{
    Canvas canvas = canvas_create(10, 10, WHITE);
    draw_line(&canvas, 1, 0, 3, 9, 1, RED);
    assert(count_color(&canvas, RED) == 10);
    for (int y = 0; y < 10; y++) {
        int in_row = 0;
        for (int x = 0; x < 10; x++) {
            in_row += color_equal(canvas_get(&canvas, x, y), RED);
        }
        assert(in_row == 1);
    }
    canvas_destroy(&canvas);
}

static void test_thick_brush_is_clipped_at_border(void)
{
    Canvas canvas = canvas_create(5, 5, WHITE);
    draw_line(&canvas, 0, 0, 0, 0, 3, RED);
    assert(count_color(&canvas, RED) == 4);
    /* A 5 x 5 brush centred on the corner keeps only its 3 x 3 part inside. */
    draw_line(&canvas, 0, 4, 0, 4, 5, BLUE);
    assert(count_color(&canvas, BLUE) == 9);
    canvas_destroy(&canvas);
}

static void test_rectangle_outline(void)
{
    Canvas canvas = canvas_create(10, 10, WHITE);
    draw_rect(&canvas, 2, 2, 5, 4, 1, RED);
    /* A 4 x 3 rectangle has 2 * (4 + 3) - 4 = 10 border pixels. */
    assert(count_color(&canvas, RED) == 10);
    assert(color_equal(canvas_get(&canvas, 3, 3), WHITE));
    assert(color_equal(canvas_get(&canvas, 5, 4), RED));
    canvas_destroy(&canvas);
}

static void test_flood_fill_stops_at_border(void)
{
    /* A 5 x 5 box of red walls; the inside is 3 x 3. */
    Canvas canvas = canvas_create(7, 7, WHITE);
    draw_rect(&canvas, 1, 1, 5, 5, 1, RED);
    flood_fill(&canvas, 3, 3, BLUE);
    assert(count_color(&canvas, BLUE) == 9);
    assert(count_color(&canvas, RED) == 16);
    assert(color_equal(canvas_get(&canvas, 0, 0), WHITE));
    assert(color_equal(canvas_get(&canvas, 6, 6), WHITE));
    canvas_destroy(&canvas);
}

static void test_flood_fill_same_color_does_nothing(void)
{
    Canvas canvas = canvas_create(4, 4, WHITE);
    flood_fill(&canvas, 1, 1, WHITE);
    assert(count_color(&canvas, WHITE) == 16);
    flood_fill(&canvas, 9, 9, RED);
    assert(count_color(&canvas, RED) == 0);
    canvas_destroy(&canvas);
}

static void test_flood_fill_whole_big_canvas(void)
{
    /* Far deeper than any recursive fill could go on the call stack. */
    Canvas canvas = canvas_create(320, 240, WHITE);
    flood_fill(&canvas, 0, 0, RED);
    assert(count_color(&canvas, RED) == 320 * 240);
    canvas_destroy(&canvas);
}

static void test_undo_restores_previous_image(void)
{
    History history;
    history_init(&history);
    Canvas canvas = canvas_create(4, 4, WHITE);

    history_push(&history, &canvas);
    canvas_set(&canvas, 0, 0, RED);
    assert(history_undo(&history, &canvas) == 1);
    assert(color_equal(canvas_get(&canvas, 0, 0), WHITE));
    assert(history_undo(&history, &canvas) == 0);

    canvas_destroy(&canvas);
    history_free(&history);
}

static void test_undo_history_drops_oldest(void)
{
    History history;
    history_init(&history);
    Canvas canvas = canvas_create(2, 2, WHITE);
    for (int i = 0; i < HISTORY_DEPTH + 5; i++) {
        canvas_set(&canvas, 0, 0, RED);
        history_push(&history, &canvas);
    }
    assert(history.count == HISTORY_DEPTH);
    int undone = 0;
    while (history_undo(&history, &canvas)) {
        undone++;
    }
    assert(undone == HISTORY_DEPTH);
    canvas_destroy(&canvas);
    history_free(&history);
}

static void test_copy_is_independent(void)
{
    Canvas a = canvas_create(3, 3, WHITE);
    Canvas b = canvas_create(3, 3, WHITE);
    canvas_set(&a, 1, 1, RED);
    canvas_copy_into(&b, &a);
    canvas_set(&a, 0, 0, BLUE);
    assert(color_equal(canvas_get(&b, 1, 1), RED));
    assert(color_equal(canvas_get(&b, 0, 0), WHITE));
    canvas_destroy(&a);
    canvas_destroy(&b);
}

int main(void)
{
    test_new_canvas_is_white();
    test_set_and_get_ignore_borders();
    test_horizontal_line();
    test_diagonal_line_and_reverse();
    test_steep_line_has_one_pixel_per_row();
    test_thick_brush_is_clipped_at_border();
    test_rectangle_outline();
    test_flood_fill_stops_at_border();
    test_flood_fill_same_color_does_nothing();
    test_flood_fill_whole_big_canvas();
    test_undo_restores_previous_image();
    test_undo_history_drops_oldest();
    test_copy_is_independent();
    printf("All graphics_editor logic tests passed.\n");
    return 0;
}
