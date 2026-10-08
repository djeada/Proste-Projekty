/* The paint logic: canvas pixels, drawing primitives and the undo history. No SDL here. */
#ifndef GRAPHICS_EDITOR_H
#define GRAPHICS_EDITOR_H

#include <stdint.h>

#define HISTORY_DEPTH 20

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

/* Pixels are stored row by row: pixel (x, y) is pixels[y * width + x]. */
typedef struct {
    int width;
    int height;
    Color *pixels;
} Canvas;

/* Snapshots of the canvas, oldest first. */
typedef struct {
    Canvas snapshots[HISTORY_DEPTH];
    int count;
} History;

Canvas canvas_create(int width, int height, Color fill);
void canvas_destroy(Canvas *canvas);
void canvas_copy_into(Canvas *dst, const Canvas *src);
int canvas_inside(const Canvas *canvas, int x, int y);
Color canvas_get(const Canvas *canvas, int x, int y);
void canvas_set(Canvas *canvas, int x, int y, Color color);
void canvas_clear(Canvas *canvas, Color color);
int color_equal(Color a, Color b);

void draw_line(Canvas *canvas, int x0, int y0, int x1, int y1, int width, Color color);
void draw_rect(Canvas *canvas, int x0, int y0, int x1, int y1, int width, Color color);
void flood_fill(Canvas *canvas, int x, int y, Color color);

void history_init(History *history);
void history_push(History *history, const Canvas *canvas);
int history_undo(History *history, Canvas *canvas);
void history_free(History *history);

#endif /* GRAPHICS_EDITOR_H */
