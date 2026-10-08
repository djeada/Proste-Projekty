/* The paint logic: canvas pixels, drawing primitives and the undo history. No SDL here. */
#include "graphics_editor.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Canvas canvas_create(int width, int height, Color fill)
{
    Canvas canvas;
    canvas.width = width;
    canvas.height = height;
    canvas.pixels = malloc(sizeof(Color) * (size_t)width * (size_t)height);
    assert(canvas.pixels != NULL);
    canvas_clear(&canvas, fill);
    return canvas;
}

void canvas_destroy(Canvas *canvas)
{
    free(canvas->pixels);
    canvas->pixels = NULL;
    canvas->width = 0;
    canvas->height = 0;
}

/* Copies the pixels of src into dst; both canvases must have the same size. */
void canvas_copy_into(Canvas *dst, const Canvas *src)
{
    assert(dst->width == src->width && dst->height == src->height);
    memcpy(dst->pixels, src->pixels, sizeof(Color) * (size_t)src->width * (size_t)src->height);
}

int canvas_inside(const Canvas *canvas, int x, int y)
{
    return x >= 0 && y >= 0 && x < canvas->width && y < canvas->height;
}

/* Returns black for a position outside the canvas. */
Color canvas_get(const Canvas *canvas, int x, int y)
{
    Color black = {0, 0, 0};
    if (!canvas_inside(canvas, x, y)) {
        return black;
    }
    return canvas->pixels[y * canvas->width + x];
}

/* Positions outside the canvas are ignored, so drawing may run past the edges. */
void canvas_set(Canvas *canvas, int x, int y, Color color)
{
    if (canvas_inside(canvas, x, y)) {
        canvas->pixels[y * canvas->width + x] = color;
    }
}

void canvas_clear(Canvas *canvas, Color color)
{
    for (int i = 0; i < canvas->width * canvas->height; i++) {
        canvas->pixels[i] = color;
    }
}

int color_equal(Color a, Color b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

/* A square brush of the given width (1, 3 or 5) centred on (x, y). */
static void stamp(Canvas *canvas, int x, int y, int width, Color color)
{
    int half = width / 2;
    for (int dy = -half; dy <= half; dy++) {
        for (int dx = -half; dx <= half; dx++) {
            canvas_set(canvas, x + dx, y + dy, color);
        }
    }
}

/*
 * Bresenham's line algorithm: step along the longer axis one pixel at a time and
 * use an error counter to decide when to also step along the shorter axis.
 * err holds how far the ideal line is from the current pixel (scaled by 2 in e2).
 */
void draw_line(Canvas *canvas, int x0, int y0, int x1, int y1, int width, Color color)
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    for (;;) {
        stamp(canvas, x0, y0, width, color);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/* The outline of the rectangle whose opposite corners are (x0, y0) and (x1, y1). */
void draw_rect(Canvas *canvas, int x0, int y0, int x1, int y1, int width, Color color)
{
    draw_line(canvas, x0, y0, x1, y0, width, color);
    draw_line(canvas, x1, y0, x1, y1, width, color);
    draw_line(canvas, x1, y1, x0, y1, width, color);
    draw_line(canvas, x0, y1, x0, y0, width, color);
}

/*
 * Iterative flood fill. Instead of recursion (which can overflow the call stack on a
 * big canvas) the pixels still to visit go on an explicit stack. A pixel is painted
 * as soon as it is pushed, so it is never pushed twice and the stack never holds more
 * than width * height entries.
 */
void flood_fill(Canvas *canvas, int x, int y, Color color)
{
    static const int dx[4] = {1, -1, 0, 0};
    static const int dy[4] = {0, 0, 1, -1};

    if (!canvas_inside(canvas, x, y)) {
        return;
    }
    Color target = canvas_get(canvas, x, y);
    if (color_equal(target, color)) {
        return;
    }

    int *stack = malloc(sizeof(int) * (size_t)canvas->width * (size_t)canvas->height);
    assert(stack != NULL);
    int top = 0;
    canvas_set(canvas, x, y, color);
    stack[top++] = y * canvas->width + x;

    while (top > 0) {
        int index = stack[--top];
        int cx = index % canvas->width;
        int cy = index / canvas->width;
        for (int i = 0; i < 4; i++) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (canvas_inside(canvas, nx, ny) && color_equal(canvas_get(canvas, nx, ny), target)) {
                canvas_set(canvas, nx, ny, color);
                stack[top++] = ny * canvas->width + nx;
            }
        }
    }
    free(stack);
}

void history_init(History *history)
{
    history->count = 0;
}

/* Saves a copy of the canvas. When the history is full the oldest snapshot is dropped. */
void history_push(History *history, const Canvas *canvas)
{
    if (history->count == HISTORY_DEPTH) {
        canvas_destroy(&history->snapshots[0]);
        memmove(&history->snapshots[0], &history->snapshots[1],
                sizeof(Canvas) * (HISTORY_DEPTH - 1));
        history->count--;
    }
    Canvas copy = canvas_create(canvas->width, canvas->height, canvas_get(canvas, 0, 0));
    canvas_copy_into(&copy, canvas);
    history->snapshots[history->count++] = copy;
}

/* Replaces the canvas with the most recent snapshot. Returns 0 if there is none. */
int history_undo(History *history, Canvas *canvas)
{
    if (history->count == 0) {
        return 0;
    }
    canvas_destroy(canvas);
    *canvas = history->snapshots[--history->count];
    return 1;
}

void history_free(History *history)
{
    for (int i = 0; i < history->count; i++) {
        canvas_destroy(&history->snapshots[i]);
    }
    history->count = 0;
}
