#ifndef GRAPHICS_EDITOR_H
#define GRAPHICS_EDITOR_H

#include <stdint.h>

#define MAX_WIDTH 1024
#define MAX_HEIGHT 1024
#define MAX_FILENAME 256

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

typedef struct {
    Pixel *data;
    int width;
    int height;
    char filename[MAX_FILENAME];
    int modified;
} Image;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Selection;

// Image management
Image *image_create(int width, int height);
void image_free(Image *img);
Image *image_copy(const Image *src);
int image_set_pixel(Image *img, int x, int y, Pixel color);
Pixel image_get_pixel(const Image *img, int x, int y);
void image_fill(Image *img, Pixel color);

// Basic operations
int image_resize(Image *img, int new_width, int new_height);
Image *image_crop(const Image *img, int x, int y, int width, int height);
Image *image_rotate_90(const Image *img);
Image *image_rotate_180(const Image *img);
Image *image_flip_horizontal(const Image *img);
Image *image_flip_vertical(const Image *img);

// Drawing tools
void image_draw_pixel(Image *img, int x, int y, Pixel color);
void image_draw_line(Image *img, int x1, int y1, int x2, int y2, Pixel color);
void image_draw_rect(Image *img, int x, int y, int width, int height, Pixel color);
void image_fill_rect(Image *img, int x, int y, int width, int height, Pixel color);
void image_draw_circle(Image *img, int cx, int cy, int radius, Pixel color);
void image_fill_circle(Image *img, int cx, int cy, int radius, Pixel color);
void image_bucket_fill(Image *img, int x, int y, Pixel new_color);

// Filters and effects
void image_grayscale(Image *img);
void image_invert(Image *img);
void image_brightness(Image *img, int delta);
void image_contrast(Image *img, float factor);

// File operations (PPM format for simplicity)
int image_save_ppm(const Image *img, const char *filename);
Image *image_load_ppm(const char *filename);

// Utility
Pixel pixel_create(uint8_t r, uint8_t g, uint8_t b);
int pixels_equal(Pixel a, Pixel b);
void image_print_ascii(const Image *img);

#endif // GRAPHICS_EDITOR_H
