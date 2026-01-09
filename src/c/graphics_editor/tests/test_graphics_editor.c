#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/graphics_editor.h"

void test_pixel_create(void) {
    Pixel p = pixel_create(100, 150, 200);
    assert(p.r == 100);
    assert(p.g == 150);
    assert(p.b == 200);
}

void test_pixels_equal(void) {
    Pixel a = pixel_create(255, 128, 64);
    Pixel b = pixel_create(255, 128, 64);
    Pixel c = pixel_create(0, 0, 0);
    
    assert(pixels_equal(a, b) == 1);
    assert(pixels_equal(a, c) == 0);
}

void test_image_create(void) {
    Image *img = image_create(100, 50);
    assert(img != NULL);
    assert(img->width == 100);
    assert(img->height == 50);
    assert(img->data != NULL);
    image_free(img);
}

void test_image_create_invalid(void) {
    assert(image_create(0, 50) == NULL);
    assert(image_create(100, 0) == NULL);
    assert(image_create(-10, 50) == NULL);
    assert(image_create(MAX_WIDTH + 1, 50) == NULL);
}

void test_image_set_get_pixel(void) {
    Image *img = image_create(10, 10);
    Pixel red = pixel_create(255, 0, 0);
    
    assert(image_set_pixel(img, 5, 5, red) == 1);
    Pixel p = image_get_pixel(img, 5, 5);
    assert(p.r == 255 && p.g == 0 && p.b == 0);
    
    // Out of bounds
    assert(image_set_pixel(img, 100, 100, red) == 0);
    
    image_free(img);
}

void test_image_fill(void) {
    Image *img = image_create(10, 10);
    Pixel blue = pixel_create(0, 0, 255);
    
    image_fill(img, blue);
    
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            Pixel p = image_get_pixel(img, x, y);
            assert(p.r == 0 && p.g == 0 && p.b == 255);
        }
    }
    
    image_free(img);
}

void test_image_copy(void) {
    Image *src = image_create(20, 20);
    Pixel green = pixel_create(0, 255, 0);
    image_fill(src, green);
    
    Image *dst = image_copy(src);
    assert(dst != NULL);
    assert(dst->width == src->width);
    assert(dst->height == src->height);
    
    Pixel p = image_get_pixel(dst, 10, 10);
    assert(p.r == 0 && p.g == 255 && p.b == 0);
    
    image_free(src);
    image_free(dst);
}

void test_image_crop(void) {
    Image *img = image_create(100, 100);
    Pixel white = pixel_create(255, 255, 255);
    image_fill(img, white);
    
    // Draw a red region
    Pixel red = pixel_create(255, 0, 0);
    image_fill_rect(img, 10, 10, 20, 20, red);
    
    Image *cropped = image_crop(img, 10, 10, 20, 20);
    assert(cropped != NULL);
    assert(cropped->width == 20);
    assert(cropped->height == 20);
    
    Pixel p = image_get_pixel(cropped, 0, 0);
    assert(p.r == 255 && p.g == 0 && p.b == 0);
    
    image_free(img);
    image_free(cropped);
}

void test_image_rotate_180(void) {
    Image *img = image_create(10, 10);
    Pixel black = pixel_create(0, 0, 0);
    Pixel white = pixel_create(255, 255, 255);
    image_fill(img, black);
    image_set_pixel(img, 0, 0, white);
    
    Image *rotated = image_rotate_180(img);
    assert(rotated != NULL);
    
    Pixel p = image_get_pixel(rotated, 9, 9);
    assert(p.r == 255 && p.g == 255 && p.b == 255);
    
    image_free(img);
    image_free(rotated);
}

void test_image_flip_horizontal(void) {
    Image *img = image_create(10, 10);
    Pixel black = pixel_create(0, 0, 0);
    Pixel white = pixel_create(255, 255, 255);
    image_fill(img, black);
    image_set_pixel(img, 0, 5, white);
    
    Image *flipped = image_flip_horizontal(img);
    assert(flipped != NULL);
    
    Pixel p = image_get_pixel(flipped, 9, 5);
    assert(p.r == 255 && p.g == 255 && p.b == 255);
    
    image_free(img);
    image_free(flipped);
}

void test_image_draw_line(void) {
    Image *img = image_create(20, 20);
    Pixel black = pixel_create(0, 0, 0);
    Pixel red = pixel_create(255, 0, 0);
    image_fill(img, black);
    
    image_draw_line(img, 0, 0, 19, 19, red);
    
    // Diagonal line should have pixels set
    Pixel p = image_get_pixel(img, 10, 10);
    assert(p.r == 255);
    
    image_free(img);
}

void test_image_grayscale(void) {
    Image *img = image_create(10, 10);
    Pixel color = pixel_create(100, 150, 200);
    image_fill(img, color);
    
    image_grayscale(img);
    
    Pixel p = image_get_pixel(img, 5, 5);
    assert(p.r == p.g && p.g == p.b);
    
    image_free(img);
}

void test_image_invert(void) {
    Image *img = image_create(10, 10);
    Pixel color = pixel_create(100, 50, 200);
    image_fill(img, color);
    
    image_invert(img);
    
    Pixel p = image_get_pixel(img, 5, 5);
    assert(p.r == 155);
    assert(p.g == 205);
    assert(p.b == 55);
    
    image_free(img);
}

void test_image_resize(void) {
    Image *img = image_create(100, 100);
    Pixel white = pixel_create(255, 255, 255);
    image_fill(img, white);
    
    assert(image_resize(img, 50, 50) == 1);
    assert(img->width == 50);
    assert(img->height == 50);
    
    image_free(img);
}

int main(void) {
    test_pixel_create();
    test_pixels_equal();
    test_image_create();
    test_image_create_invalid();
    test_image_set_get_pixel();
    test_image_fill();
    test_image_copy();
    test_image_crop();
    test_image_rotate_180();
    test_image_flip_horizontal();
    test_image_draw_line();
    test_image_grayscale();
    test_image_invert();
    test_image_resize();
    printf("All tests passed!\n");
    return 0;
}
