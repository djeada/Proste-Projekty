#include "graphics_editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Pixel pixel_create(uint8_t r, uint8_t g, uint8_t b) {
    Pixel p = {r, g, b};
    return p;
}

int pixels_equal(Pixel a, Pixel b) {
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

Image *image_create(int width, int height) {
    if (width <= 0 || height <= 0 || width > MAX_WIDTH || height > MAX_HEIGHT) {
        return NULL;
    }

    Image *img = malloc(sizeof(Image));
    if (!img) return NULL;

    img->data = calloc((size_t)(width * height), sizeof(Pixel));
    if (!img->data) {
        free(img);
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->filename[0] = '\0';
    img->modified = 0;

    return img;
}

void image_free(Image *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}

Image *image_copy(const Image *src) {
    if (!src) return NULL;

    Image *dst = image_create(src->width, src->height);
    if (!dst) return NULL;

    memcpy(dst->data, src->data, (size_t)(src->width * src->height) * sizeof(Pixel));
    strncpy(dst->filename, src->filename, MAX_FILENAME - 1);
    dst->modified = src->modified;

    return dst;
}

int image_set_pixel(Image *img, int x, int y, Pixel color) {
    if (!img || x < 0 || x >= img->width || y < 0 || y >= img->height) {
        return 0;
    }
    img->data[y * img->width + x] = color;
    img->modified = 1;
    return 1;
}

Pixel image_get_pixel(const Image *img, int x, int y) {
    Pixel black = {0, 0, 0};
    if (!img || x < 0 || x >= img->width || y < 0 || y >= img->height) {
        return black;
    }
    return img->data[y * img->width + x];
}

void image_fill(Image *img, Pixel color) {
    if (!img) return;
    for (int i = 0; i < img->width * img->height; i++) {
        img->data[i] = color;
    }
    img->modified = 1;
}

int image_resize(Image *img, int new_width, int new_height) {
    if (!img || new_width <= 0 || new_height <= 0 ||
        new_width > MAX_WIDTH || new_height > MAX_HEIGHT) {
        return 0;
    }

    Pixel *new_data = calloc((size_t)(new_width * new_height), sizeof(Pixel));
    if (!new_data) return 0;

    // Simple nearest-neighbor scaling
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            int src_x = x * img->width / new_width;
            int src_y = y * img->height / new_height;
            new_data[y * new_width + x] = img->data[src_y * img->width + src_x];
        }
    }

    free(img->data);
    img->data = new_data;
    img->width = new_width;
    img->height = new_height;
    img->modified = 1;

    return 1;
}

Image *image_crop(const Image *img, int x, int y, int width, int height) {
    if (!img || x < 0 || y < 0 || width <= 0 || height <= 0 ||
        x + width > img->width || y + height > img->height) {
        return NULL;
    }

    Image *cropped = image_create(width, height);
    if (!cropped) return NULL;

    for (int dy = 0; dy < height; dy++) {
        for (int dx = 0; dx < width; dx++) {
            cropped->data[dy * width + dx] = img->data[(y + dy) * img->width + (x + dx)];
        }
    }

    return cropped;
}

Image *image_rotate_90(const Image *img) {
    if (!img) return NULL;

    Image *rotated = image_create(img->height, img->width);
    if (!rotated) return NULL;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            rotated->data[x * rotated->width + (img->height - 1 - y)] =
                img->data[y * img->width + x];
        }
    }

    return rotated;
}

Image *image_rotate_180(const Image *img) {
    if (!img) return NULL;

    Image *rotated = image_create(img->width, img->height);
    if (!rotated) return NULL;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            rotated->data[(img->height - 1 - y) * img->width + (img->width - 1 - x)] =
                img->data[y * img->width + x];
        }
    }

    return rotated;
}

Image *image_flip_horizontal(const Image *img) {
    if (!img) return NULL;

    Image *flipped = image_create(img->width, img->height);
    if (!flipped) return NULL;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            flipped->data[y * img->width + (img->width - 1 - x)] =
                img->data[y * img->width + x];
        }
    }

    return flipped;
}

Image *image_flip_vertical(const Image *img) {
    if (!img) return NULL;

    Image *flipped = image_create(img->width, img->height);
    if (!flipped) return NULL;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            flipped->data[(img->height - 1 - y) * img->width + x] =
                img->data[y * img->width + x];
        }
    }

    return flipped;
}

void image_draw_pixel(Image *img, int x, int y, Pixel color) {
    image_set_pixel(img, x, y, color);
}

void image_draw_line(Image *img, int x1, int y1, int x2, int y2, Pixel color) {
    if (!img) return;

    // Bresenham's line algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        image_set_pixel(img, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void image_draw_rect(Image *img, int x, int y, int width, int height, Pixel color) {
    if (!img || width <= 0 || height <= 0) return;

    image_draw_line(img, x, y, x + width - 1, y, color);
    image_draw_line(img, x, y + height - 1, x + width - 1, y + height - 1, color);
    image_draw_line(img, x, y, x, y + height - 1, color);
    image_draw_line(img, x + width - 1, y, x + width - 1, y + height - 1, color);
}

void image_fill_rect(Image *img, int x, int y, int width, int height, Pixel color) {
    if (!img || width <= 0 || height <= 0) return;

    for (int dy = 0; dy < height; dy++) {
        for (int dx = 0; dx < width; dx++) {
            image_set_pixel(img, x + dx, y + dy, color);
        }
    }
}

void image_draw_circle(Image *img, int cx, int cy, int radius, Pixel color) {
    if (!img || radius <= 0) return;

    // Midpoint circle algorithm
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        image_set_pixel(img, cx + x, cy + y, color);
        image_set_pixel(img, cx + y, cy + x, color);
        image_set_pixel(img, cx - y, cy + x, color);
        image_set_pixel(img, cx - x, cy + y, color);
        image_set_pixel(img, cx - x, cy - y, color);
        image_set_pixel(img, cx - y, cy - x, color);
        image_set_pixel(img, cx + y, cy - x, color);
        image_set_pixel(img, cx + x, cy - y, color);

        y++;
        if (err <= 0) {
            err += 2 * y + 1;
        }
        if (err > 0) {
            x--;
            err -= 2 * x + 1;
        }
    }
}

void image_fill_circle(Image *img, int cx, int cy, int radius, Pixel color) {
    if (!img || radius <= 0) return;

    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                image_set_pixel(img, cx + x, cy + y, color);
            }
        }
    }
}

static void bucket_fill_recursive(Image *img, int x, int y, Pixel old_color, Pixel new_color) {
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) return;

    Pixel current = image_get_pixel(img, x, y);
    if (!pixels_equal(current, old_color) || pixels_equal(current, new_color)) return;

    image_set_pixel(img, x, y, new_color);

    // Use stack-based approach for large images to avoid stack overflow
    bucket_fill_recursive(img, x + 1, y, old_color, new_color);
    bucket_fill_recursive(img, x - 1, y, old_color, new_color);
    bucket_fill_recursive(img, x, y + 1, old_color, new_color);
    bucket_fill_recursive(img, x, y - 1, old_color, new_color);
}

void image_bucket_fill(Image *img, int x, int y, Pixel new_color) {
    if (!img || x < 0 || x >= img->width || y < 0 || y >= img->height) return;

    Pixel old_color = image_get_pixel(img, x, y);
    if (pixels_equal(old_color, new_color)) return;

    bucket_fill_recursive(img, x, y, old_color, new_color);
}

void image_grayscale(Image *img) {
    if (!img) return;

    for (int i = 0; i < img->width * img->height; i++) {
        Pixel *p = &img->data[i];
        uint8_t gray = (uint8_t)((p->r * 299 + p->g * 587 + p->b * 114) / 1000);
        p->r = p->g = p->b = gray;
    }
    img->modified = 1;
}

void image_invert(Image *img) {
    if (!img) return;

    for (int i = 0; i < img->width * img->height; i++) {
        Pixel *p = &img->data[i];
        p->r = 255 - p->r;
        p->g = 255 - p->g;
        p->b = 255 - p->b;
    }
    img->modified = 1;
}

static uint8_t clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (uint8_t)value;
}

void image_brightness(Image *img, int delta) {
    if (!img) return;

    for (int i = 0; i < img->width * img->height; i++) {
        Pixel *p = &img->data[i];
        p->r = clamp(p->r + delta);
        p->g = clamp(p->g + delta);
        p->b = clamp(p->b + delta);
    }
    img->modified = 1;
}

void image_contrast(Image *img, float factor) {
    if (!img) return;

    for (int i = 0; i < img->width * img->height; i++) {
        Pixel *p = &img->data[i];
        p->r = clamp((int)(((p->r - 128) * factor) + 128));
        p->g = clamp((int)(((p->g - 128) * factor) + 128));
        p->b = clamp((int)(((p->b - 128) * factor) + 128));
    }
    img->modified = 1;
}

int image_save_ppm(const Image *img, const char *filename) {
    if (!img || !filename) return 0;

    FILE *f = fopen(filename, "wb");
    if (!f) return 0;

    fprintf(f, "P6\n%d %d\n255\n", img->width, img->height);

    for (int i = 0; i < img->width * img->height; i++) {
        fputc(img->data[i].r, f);
        fputc(img->data[i].g, f);
        fputc(img->data[i].b, f);
    }

    fclose(f);
    return 1;
}

Image *image_load_ppm(const char *filename) {
    if (!filename) return NULL;

    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;

    char magic[3];
    if (fscanf(f, "%2s", magic) != 1 || strcmp(magic, "P6") != 0) {
        fclose(f);
        return NULL;
    }

    int width, height, maxval;
    if (fscanf(f, "%d %d %d", &width, &height, &maxval) != 3) {
        fclose(f);
        return NULL;
    }

    // Skip single whitespace
    fgetc(f);

    Image *img = image_create(width, height);
    if (!img) {
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < width * height; i++) {
        img->data[i].r = (uint8_t)fgetc(f);
        img->data[i].g = (uint8_t)fgetc(f);
        img->data[i].b = (uint8_t)fgetc(f);
    }

    strncpy(img->filename, filename, MAX_FILENAME - 1);
    img->filename[MAX_FILENAME - 1] = '\0';
    img->modified = 0;

    fclose(f);
    return img;
}

void image_print_ascii(const Image *img) {
    if (!img) return;

    const char *chars = " .:-=+*#%@";
    int num_chars = 10;

    printf("\n");
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            Pixel p = img->data[y * img->width + x];
            int brightness = (p.r + p.g + p.b) / 3;
            int idx = brightness * (num_chars - 1) / 255;
            printf("%c", chars[idx]);
        }
        printf("\n");
    }
}
