#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics_editor.h"

static Image *current_image = NULL;
static Pixel current_color = {255, 255, 255};

void print_menu(void) {
    printf("\n=== Simple Graphics Editor ===\n");
    printf("1.  New image\n");
    printf("2.  Load image (PPM)\n");
    printf("3.  Save image (PPM)\n");
    printf("4.  Set color (RGB)\n");
    printf("5.  Draw pixel\n");
    printf("6.  Draw line\n");
    printf("7.  Draw rectangle\n");
    printf("8.  Fill rectangle\n");
    printf("9.  Draw circle\n");
    printf("10. Fill circle\n");
    printf("11. Bucket fill\n");
    printf("12. Rotate 90°\n");
    printf("13. Rotate 180°\n");
    printf("14. Flip horizontal\n");
    printf("15. Flip vertical\n");
    printf("16. Resize\n");
    printf("17. Crop\n");
    printf("18. Grayscale\n");
    printf("19. Invert colors\n");
    printf("20. Adjust brightness\n");
    printf("21. Adjust contrast\n");
    printf("22. View (ASCII)\n");
    printf("23. Image info\n");
    printf("0.  Exit\n");
    printf("Choice: ");
}

int read_int(const char *prompt) {
    char input[64];
    printf("%s", prompt);
    if (!fgets(input, sizeof(input), stdin)) return 0;
    return atoi(input);
}

void read_string(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
    }
}

int main(void) {
    char input[256];
    char filename[MAX_FILENAME];
    int running = 1;

    printf("Welcome to Simple Graphics Editor!\n");
    printf("This editor works with PPM image format.\n");

    while (running) {
        print_menu();
        if (!fgets(input, sizeof(input), stdin)) break;

        int choice = atoi(input);

        switch (choice) {
            case 0:
                running = 0;
                break;

            case 1: {
                int w = read_int("Width: ");
                int h = read_int("Height: ");
                if (w > 0 && h > 0 && w <= MAX_WIDTH && h <= MAX_HEIGHT) {
                    image_free(current_image);
                    current_image = image_create(w, h);
                    if (current_image) {
                        Pixel white = {255, 255, 255};
                        image_fill(current_image, white);
                        printf("Created new %dx%d image.\n", w, h);
                    }
                } else {
                    printf("Invalid dimensions.\n");
                }
                break;
            }

            case 2:
                read_string("Filename: ", filename, sizeof(filename));
                image_free(current_image);
                current_image = image_load_ppm(filename);
                if (current_image) {
                    printf("Loaded %dx%d image.\n", current_image->width, current_image->height);
                } else {
                    printf("Failed to load image.\n");
                }
                break;

            case 3:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    read_string("Filename: ", filename, sizeof(filename));
                    if (image_save_ppm(current_image, filename)) {
                        printf("Image saved.\n");
                    } else {
                        printf("Failed to save image.\n");
                    }
                }
                break;

            case 4: {
                int r = read_int("Red (0-255): ");
                int g = read_int("Green (0-255): ");
                int b = read_int("Blue (0-255): ");
                current_color = pixel_create((uint8_t)r, (uint8_t)g, (uint8_t)b);
                printf("Color set to RGB(%d, %d, %d).\n", r, g, b);
                break;
            }

            case 5:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int x = read_int("X: ");
                    int y = read_int("Y: ");
                    image_draw_pixel(current_image, x, y, current_color);
                    printf("Pixel drawn.\n");
                }
                break;

            case 6:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int x1 = read_int("X1: ");
                    int y1 = read_int("Y1: ");
                    int x2 = read_int("X2: ");
                    int y2 = read_int("Y2: ");
                    image_draw_line(current_image, x1, y1, x2, y2, current_color);
                    printf("Line drawn.\n");
                }
                break;

            case 7:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int x = read_int("X: ");
                    int y = read_int("Y: ");
                    int w = read_int("Width: ");
                    int h = read_int("Height: ");
                    image_draw_rect(current_image, x, y, w, h, current_color);
                    printf("Rectangle drawn.\n");
                }
                break;

            case 8:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int x = read_int("X: ");
                    int y = read_int("Y: ");
                    int w = read_int("Width: ");
                    int h = read_int("Height: ");
                    image_fill_rect(current_image, x, y, w, h, current_color);
                    printf("Rectangle filled.\n");
                }
                break;

            case 9:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int cx = read_int("Center X: ");
                    int cy = read_int("Center Y: ");
                    int r = read_int("Radius: ");
                    image_draw_circle(current_image, cx, cy, r, current_color);
                    printf("Circle drawn.\n");
                }
                break;

            case 10:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int cx = read_int("Center X: ");
                    int cy = read_int("Center Y: ");
                    int r = read_int("Radius: ");
                    image_fill_circle(current_image, cx, cy, r, current_color);
                    printf("Circle filled.\n");
                }
                break;

            case 11:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int x = read_int("X: ");
                    int y = read_int("Y: ");
                    image_bucket_fill(current_image, x, y, current_color);
                    printf("Bucket fill applied.\n");
                }
                break;

            case 12:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    Image *rotated = image_rotate_90(current_image);
                    if (rotated) {
                        image_free(current_image);
                        current_image = rotated;
                        printf("Rotated 90 degrees.\n");
                    }
                }
                break;

            case 13:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    Image *rotated = image_rotate_180(current_image);
                    if (rotated) {
                        image_free(current_image);
                        current_image = rotated;
                        printf("Rotated 180 degrees.\n");
                    }
                }
                break;

            case 14:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    Image *flipped = image_flip_horizontal(current_image);
                    if (flipped) {
                        image_free(current_image);
                        current_image = flipped;
                        printf("Flipped horizontally.\n");
                    }
                }
                break;

            case 15:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    Image *flipped = image_flip_vertical(current_image);
                    if (flipped) {
                        image_free(current_image);
                        current_image = flipped;
                        printf("Flipped vertically.\n");
                    }
                }
                break;

            case 16:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int w = read_int("New width: ");
                    int h = read_int("New height: ");
                    if (image_resize(current_image, w, h)) {
                        printf("Resized to %dx%d.\n", w, h);
                    } else {
                        printf("Failed to resize.\n");
                    }
                }
                break;

            case 17:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int x = read_int("X: ");
                    int y = read_int("Y: ");
                    int w = read_int("Width: ");
                    int h = read_int("Height: ");
                    Image *cropped = image_crop(current_image, x, y, w, h);
                    if (cropped) {
                        image_free(current_image);
                        current_image = cropped;
                        printf("Cropped to %dx%d.\n", w, h);
                    } else {
                        printf("Invalid crop area.\n");
                    }
                }
                break;

            case 18:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    image_grayscale(current_image);
                    printf("Converted to grayscale.\n");
                }
                break;

            case 19:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    image_invert(current_image);
                    printf("Colors inverted.\n");
                }
                break;

            case 20:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    int delta = read_int("Brightness adjustment (-255 to 255): ");
                    image_brightness(current_image, delta);
                    printf("Brightness adjusted.\n");
                }
                break;

            case 21:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    printf("Contrast factor (0.5 = low, 1.0 = normal, 2.0 = high): ");
                    if (fgets(input, sizeof(input), stdin)) {
                        float factor = (float)atof(input);
                        image_contrast(current_image, factor);
                        printf("Contrast adjusted.\n");
                    }
                }
                break;

            case 22:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    image_print_ascii(current_image);
                }
                break;

            case 23:
                if (!current_image) {
                    printf("No image loaded.\n");
                } else {
                    printf("\nImage info:\n");
                    printf("Size: %dx%d\n", current_image->width, current_image->height);
                    printf("Filename: %s\n", current_image->filename[0] ? current_image->filename : "(none)");
                    printf("Modified: %s\n", current_image->modified ? "Yes" : "No");
                    printf("Current color: RGB(%d, %d, %d)\n",
                           current_color.r, current_color.g, current_color.b);
                }
                break;

            default:
                printf("Invalid choice.\n");
        }
    }

    image_free(current_image);
    printf("Goodbye!\n");
    return 0;
}
