/**
 * Graphics Editor - SDL2 GUI Implementation
 *
 * A full-featured graphical paint application with mouse-based drawing,
 * multiple tools, color selection, and image manipulation.
 *
 * Controls:
 *   Mouse: Draw with selected tool
 *   1-5: Select tools (Pencil, Line, Rectangle, Circle, Fill)
 *   C: Open color picker
 *   N: New image
 *   S: Save image (PPM)
 *   L: Load image (PPM)
 *   G: Convert to grayscale
 *   I: Invert colors
 *   R: Rotate 90 degrees
 *   H: Flip horizontal
 *   V: Flip vertical
 *   Delete/Backspace: Clear canvas
 *   Escape: Exit
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics_editor.h"

/* Window and canvas dimensions */
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define TOOLBAR_HEIGHT 60
#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600
#define COLOR_PALETTE_SIZE 40
#define TOOL_BUTTON_SIZE 50

/* Drawing tools enumeration */
typedef enum {
    TOOL_PENCIL = 0,
    TOOL_LINE,
    TOOL_RECTANGLE,
    TOOL_CIRCLE,
    TOOL_FILL,
    TOOL_COUNT
} Tool;

/* Button structure for UI elements */
typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    SDL_Color hover_color;
    int is_hovered;
    int is_selected;
    const char *label;
} Button;

/* Color palette preset colors */
static const SDL_Color palette_colors[] = {
    {0, 0, 0, 255},       /* Black */
    {255, 255, 255, 255}, /* White */
    {255, 0, 0, 255},     /* Red */
    {0, 255, 0, 255},     /* Green */
    {0, 0, 255, 255},     /* Blue */
    {255, 255, 0, 255},   /* Yellow */
    {255, 0, 255, 255},   /* Magenta */
    {0, 255, 255, 255},   /* Cyan */
    {255, 128, 0, 255},   /* Orange */
    {128, 0, 255, 255},   /* Purple */
    {255, 192, 203, 255}, /* Pink */
    {165, 42, 42, 255},   /* Brown */
    {128, 128, 128, 255}, /* Gray */
    {192, 192, 192, 255}, /* Light Gray */
    {0, 128, 0, 255},     /* Dark Green */
    {0, 0, 128, 255},     /* Navy */
};
#define PALETTE_COUNT (sizeof(palette_colors) / sizeof(palette_colors[0]))

/* Application state */
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *canvas_texture;
    Image *image;
    Pixel current_color;
    Tool current_tool;
    int is_drawing;
    int start_x;
    int start_y;
    int last_x;
    int last_y;
    int canvas_offset_x;
    int canvas_offset_y;
    Button tool_buttons[TOOL_COUNT];
    Button color_buttons[PALETTE_COUNT];
    Button current_color_display;
    int running;
} AppState;

/* Tool names for display */
static const char *tool_names[] = {"Pencil", "Line", "Rect", "Circle", "Fill"};

/* Function prototypes */
static int init_sdl(AppState *app);
static void cleanup_sdl(AppState *app);
static void init_ui(AppState *app);
static void handle_events(AppState *app);
static void render(AppState *app);
static void update_canvas_texture(AppState *app);
static void draw_toolbar(AppState *app);
static void draw_button(AppState *app, Button *btn);
static int point_in_rect(int x, int y, SDL_Rect *rect);
static void handle_canvas_click(AppState *app, int x, int y);
static void handle_canvas_drag(AppState *app, int x, int y);
static void handle_canvas_release(AppState *app, int x, int y);
static void handle_tool_click(AppState *app, int tool_index);
static void handle_color_click(AppState *app, int color_index);
static void new_image(AppState *app);
static void clear_canvas(AppState *app);
static void save_image_dialog(AppState *app);
static void load_image_dialog(AppState *app);

/**
 * Initialize SDL2 and create window/renderer
 */
static int init_sdl(AppState *app) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 0;
    }

    app->window = SDL_CreateWindow("Graphics Editor",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_WINDOW_SHOWN);
    if (!app->window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        return 0;
    }

    app->renderer = SDL_CreateRenderer(app->window, -1,
                                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!app->renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        return 0;
    }

    /* Create canvas texture */
    app->canvas_texture = SDL_CreateTexture(app->renderer,
                                            SDL_PIXELFORMAT_RGB24,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            CANVAS_WIDTH, CANVAS_HEIGHT);
    if (!app->canvas_texture) {
        fprintf(stderr, "Texture creation failed: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

/**
 * Clean up SDL resources
 */
static void cleanup_sdl(AppState *app) {
    if (app->canvas_texture) SDL_DestroyTexture(app->canvas_texture);
    if (app->renderer) SDL_DestroyRenderer(app->renderer);
    if (app->window) SDL_DestroyWindow(app->window);
    image_free(app->image);
    SDL_Quit();
}

/**
 * Initialize UI elements (buttons, layout)
 */
static void init_ui(AppState *app) {
    int x_offset = 10;

    /* Initialize tool buttons */
    for (int i = 0; i < TOOL_COUNT; i++) {
        app->tool_buttons[i].rect.x = x_offset + i * (TOOL_BUTTON_SIZE + 5);
        app->tool_buttons[i].rect.y = 5;
        app->tool_buttons[i].rect.w = TOOL_BUTTON_SIZE;
        app->tool_buttons[i].rect.h = TOOL_BUTTON_SIZE;
        app->tool_buttons[i].color = (SDL_Color){200, 200, 200, 255};
        app->tool_buttons[i].hover_color = (SDL_Color){170, 170, 170, 255};
        app->tool_buttons[i].is_hovered = 0;
        app->tool_buttons[i].is_selected = (i == 0);
        app->tool_buttons[i].label = tool_names[i];
    }

    /* Initialize color palette buttons */
    x_offset = 300;
    for (int i = 0; i < (int)PALETTE_COUNT; i++) {
        int row = i / 8;
        int col = i % 8;
        app->color_buttons[i].rect.x = x_offset + col * 25;
        app->color_buttons[i].rect.y = 5 + row * 25;
        app->color_buttons[i].rect.w = 22;
        app->color_buttons[i].rect.h = 22;
        app->color_buttons[i].color = palette_colors[i];
        app->color_buttons[i].hover_color = palette_colors[i];
        app->color_buttons[i].is_hovered = 0;
        app->color_buttons[i].is_selected = 0;
        app->color_buttons[i].label = NULL;
    }

    /* Current color display */
    app->current_color_display.rect.x = 520;
    app->current_color_display.rect.y = 10;
    app->current_color_display.rect.w = 40;
    app->current_color_display.rect.h = 40;

    /* Calculate canvas position (centered horizontally, below toolbar) */
    app->canvas_offset_x = (WINDOW_WIDTH - CANVAS_WIDTH) / 2;
    app->canvas_offset_y = TOOLBAR_HEIGHT + 10;
}

/**
 * Check if point is inside rectangle
 */
static int point_in_rect(int x, int y, SDL_Rect *rect) {
    return x >= rect->x && x < rect->x + rect->w && y >= rect->y && y < rect->y + rect->h;
}

/**
 * Handle SDL events
 */
static void handle_events(AppState *app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                app->running = 0;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        app->running = 0;
                        break;
                    case SDLK_1:
                        handle_tool_click(app, TOOL_PENCIL);
                        break;
                    case SDLK_2:
                        handle_tool_click(app, TOOL_LINE);
                        break;
                    case SDLK_3:
                        handle_tool_click(app, TOOL_RECTANGLE);
                        break;
                    case SDLK_4:
                        handle_tool_click(app, TOOL_CIRCLE);
                        break;
                    case SDLK_5:
                        handle_tool_click(app, TOOL_FILL);
                        break;
                    case SDLK_n:
                        new_image(app);
                        break;
                    case SDLK_s:
                        save_image_dialog(app);
                        break;
                    case SDLK_l:
                        load_image_dialog(app);
                        break;
                    case SDLK_g:
                        image_grayscale(app->image);
                        update_canvas_texture(app);
                        break;
                    case SDLK_i:
                        image_invert(app->image);
                        update_canvas_texture(app);
                        break;
                    case SDLK_r:
                        {
                            Image *rotated = image_rotate_90(app->image);
                            if (rotated) {
                                image_free(app->image);
                                app->image = rotated;
                                update_canvas_texture(app);
                            }
                        }
                        break;
                    case SDLK_h:
                        {
                            Image *flipped = image_flip_horizontal(app->image);
                            if (flipped) {
                                image_free(app->image);
                                app->image = flipped;
                                update_canvas_texture(app);
                            }
                        }
                        break;
                    case SDLK_v:
                        {
                            Image *flipped = image_flip_vertical(app->image);
                            if (flipped) {
                                image_free(app->image);
                                app->image = flipped;
                                update_canvas_texture(app);
                            }
                        }
                        break;
                    case SDLK_DELETE:
                    case SDLK_BACKSPACE:
                        clear_canvas(app);
                        break;
                    default:
                        break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mx = event.button.x;
                    int my = event.button.y;

                    /* Check tool buttons */
                    for (int i = 0; i < TOOL_COUNT; i++) {
                        if (point_in_rect(mx, my, &app->tool_buttons[i].rect)) {
                            handle_tool_click(app, i);
                            break;
                        }
                    }

                    /* Check color buttons */
                    for (int i = 0; i < (int)PALETTE_COUNT; i++) {
                        if (point_in_rect(mx, my, &app->color_buttons[i].rect)) {
                            handle_color_click(app, i);
                            break;
                        }
                    }

                    /* Check canvas */
                    SDL_Rect canvas_rect = {app->canvas_offset_x, app->canvas_offset_y,
                                            CANVAS_WIDTH, CANVAS_HEIGHT};
                    if (point_in_rect(mx, my, &canvas_rect)) {
                        int cx = mx - app->canvas_offset_x;
                        int cy = my - app->canvas_offset_y;
                        handle_canvas_click(app, cx, cy);
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT && app->is_drawing) {
                    int mx = event.button.x;
                    int my = event.button.y;
                    int cx = mx - app->canvas_offset_x;
                    int cy = my - app->canvas_offset_y;
                    handle_canvas_release(app, cx, cy);
                }
                break;

            case SDL_MOUSEMOTION:
                {
                    int mx = event.motion.x;
                    int my = event.motion.y;

                    /* Update hover state for tool buttons */
                    for (int i = 0; i < TOOL_COUNT; i++) {
                        app->tool_buttons[i].is_hovered =
                            point_in_rect(mx, my, &app->tool_buttons[i].rect);
                    }

                    /* Update hover state for color buttons */
                    for (int i = 0; i < (int)PALETTE_COUNT; i++) {
                        app->color_buttons[i].is_hovered =
                            point_in_rect(mx, my, &app->color_buttons[i].rect);
                    }

                    /* Handle drawing drag */
                    if (app->is_drawing) {
                        SDL_Rect canvas_rect = {app->canvas_offset_x, app->canvas_offset_y,
                                                CANVAS_WIDTH, CANVAS_HEIGHT};
                        if (point_in_rect(mx, my, &canvas_rect)) {
                            int cx = mx - app->canvas_offset_x;
                            int cy = my - app->canvas_offset_y;
                            handle_canvas_drag(app, cx, cy);
                        }
                    }
                }
                break;
        }
    }
}

/**
 * Handle click on canvas
 */
static void handle_canvas_click(AppState *app, int x, int y) {
    if (x < 0 || x >= app->image->width || y < 0 || y >= app->image->height) {
        return;
    }

    app->is_drawing = 1;
    app->start_x = x;
    app->start_y = y;
    app->last_x = x;
    app->last_y = y;

    switch (app->current_tool) {
        case TOOL_PENCIL:
            image_draw_pixel(app->image, x, y, app->current_color);
            update_canvas_texture(app);
            break;
        case TOOL_FILL:
            image_bucket_fill(app->image, x, y, app->current_color);
            update_canvas_texture(app);
            app->is_drawing = 0;
            break;
        default:
            break;
    }
}

/**
 * Handle mouse drag on canvas
 */
static void handle_canvas_drag(AppState *app, int x, int y) {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= app->image->width) x = app->image->width - 1;
    if (y >= app->image->height) y = app->image->height - 1;

    if (app->current_tool == TOOL_PENCIL) {
        image_draw_line(app->image, app->last_x, app->last_y, x, y, app->current_color);
        update_canvas_texture(app);
        app->last_x = x;
        app->last_y = y;
    }
}

/**
 * Handle mouse release on canvas
 */
static void handle_canvas_release(AppState *app, int x, int y) {
    if (!app->is_drawing) return;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= app->image->width) x = app->image->width - 1;
    if (y >= app->image->height) y = app->image->height - 1;

    switch (app->current_tool) {
        case TOOL_LINE:
            image_draw_line(app->image, app->start_x, app->start_y, x, y, app->current_color);
            break;
        case TOOL_RECTANGLE:
            {
                int rx = app->start_x < x ? app->start_x : x;
                int ry = app->start_y < y ? app->start_y : y;
                int rw = abs(x - app->start_x);
                int rh = abs(y - app->start_y);
                if (rw > 0 && rh > 0) {
                    image_draw_rect(app->image, rx, ry, rw, rh, app->current_color);
                }
            }
            break;
        case TOOL_CIRCLE:
            {
                int dx = x - app->start_x;
                int dy = y - app->start_y;
                int radius = (int)SDL_sqrt((double)(dx * dx + dy * dy));
                if (radius > 0) {
                    image_draw_circle(app->image, app->start_x, app->start_y, radius,
                                      app->current_color);
                }
            }
            break;
        default:
            break;
    }

    update_canvas_texture(app);
    app->is_drawing = 0;
}

/**
 * Handle tool selection
 */
static void handle_tool_click(AppState *app, int tool_index) {
    for (int i = 0; i < TOOL_COUNT; i++) {
        app->tool_buttons[i].is_selected = (i == tool_index);
    }
    app->current_tool = (Tool)tool_index;
}

/**
 * Handle color selection from palette
 */
static void handle_color_click(AppState *app, int color_index) {
    SDL_Color c = palette_colors[color_index];
    app->current_color = pixel_create(c.r, c.g, c.b);
}

/**
 * Create a new blank image
 */
static void new_image(AppState *app) {
    image_free(app->image);
    app->image = image_create(CANVAS_WIDTH, CANVAS_HEIGHT);
    if (app->image) {
        Pixel white = {255, 255, 255};
        image_fill(app->image, white);
        update_canvas_texture(app);
    }
}

/**
 * Clear the canvas to white
 */
static void clear_canvas(AppState *app) {
    Pixel white = {255, 255, 255};
    image_fill(app->image, white);
    update_canvas_texture(app);
}

/**
 * Save image to file.
 * Note: Currently uses hardcoded filename for simplicity.
 * A proper file dialog would require platform-specific code or additional libraries.
 * TODO: Consider using tinyfiledialogs or similar for cross-platform file dialogs.
 */
static void save_image_dialog(AppState *app) {
    const char *filename = "output.ppm";
    if (image_save_ppm(app->image, filename)) {
        printf("Image saved to %s\n", filename);
    } else {
        printf("Failed to save image\n");
    }
}

/**
 * Load image from file.
 * Note: Currently uses hardcoded filename for simplicity.
 * A proper file dialog would require platform-specific code or additional libraries.
 * TODO: Consider using tinyfiledialogs or similar for cross-platform file dialogs.
 */
static void load_image_dialog(AppState *app) {
    const char *filename = "output.ppm";
    Image *loaded = image_load_ppm(filename);
    if (loaded) {
        image_free(app->image);
        app->image = loaded;
        update_canvas_texture(app);
        printf("Image loaded from %s\n", filename);
    } else {
        printf("Failed to load image from %s\n", filename);
    }
}

/**
 * Update SDL texture from image data
 */
static void update_canvas_texture(AppState *app) {
    if (!app->image || !app->canvas_texture) return;

    void *pixels;
    int pitch;

    if (SDL_LockTexture(app->canvas_texture, NULL, &pixels, &pitch) == 0) {
        uint8_t *dst = (uint8_t *)pixels;

        for (int y = 0; y < app->image->height && y < CANVAS_HEIGHT; y++) {
            for (int x = 0; x < app->image->width && x < CANVAS_WIDTH; x++) {
                Pixel p = image_get_pixel(app->image, x, y);
                int idx = y * pitch + x * 3;
                dst[idx] = p.r;
                dst[idx + 1] = p.g;
                dst[idx + 2] = p.b;
            }
        }

        SDL_UnlockTexture(app->canvas_texture);
    }
}

/**
 * Draw a button
 */
static void draw_button(AppState *app, Button *btn) {
    SDL_Color c;
    if (btn->is_selected) {
        c = (SDL_Color){100, 150, 255, 255};
    } else if (btn->is_hovered) {
        c = btn->hover_color;
    } else {
        c = btn->color;
    }

    SDL_SetRenderDrawColor(app->renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(app->renderer, &btn->rect);

    /* Draw border */
    SDL_SetRenderDrawColor(app->renderer, 50, 50, 50, 255);
    SDL_RenderDrawRect(app->renderer, &btn->rect);
}

/**
 * Draw the toolbar
 */
static void draw_toolbar(AppState *app) {
    /* Toolbar background */
    SDL_Rect toolbar_rect = {0, 0, WINDOW_WIDTH, TOOLBAR_HEIGHT};
    SDL_SetRenderDrawColor(app->renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(app->renderer, &toolbar_rect);

    /* Toolbar border */
    SDL_SetRenderDrawColor(app->renderer, 180, 180, 180, 255);
    SDL_RenderDrawLine(app->renderer, 0, TOOLBAR_HEIGHT, WINDOW_WIDTH, TOOLBAR_HEIGHT);

    /* Draw tool buttons */
    for (int i = 0; i < TOOL_COUNT; i++) {
        draw_button(app, &app->tool_buttons[i]);

        /* Draw simple tool icons */
        SDL_SetRenderDrawColor(app->renderer, 50, 50, 50, 255);
        int cx = app->tool_buttons[i].rect.x + TOOL_BUTTON_SIZE / 2;
        int cy = app->tool_buttons[i].rect.y + TOOL_BUTTON_SIZE / 2;

        switch (i) {
            case TOOL_PENCIL:
                /* Pencil icon - diagonal line */
                SDL_RenderDrawLine(app->renderer, cx - 10, cy + 10, cx + 10, cy - 10);
                SDL_RenderDrawLine(app->renderer, cx - 9, cy + 10, cx + 11, cy - 10);
                break;
            case TOOL_LINE:
                /* Line icon */
                SDL_RenderDrawLine(app->renderer, cx - 12, cy + 8, cx + 12, cy - 8);
                SDL_RenderDrawLine(app->renderer, cx - 12, cy + 9, cx + 12, cy - 7);
                break;
            case TOOL_RECTANGLE:
                {
                    /* Rectangle icon */
                    SDL_Rect icon = {cx - 12, cy - 8, 24, 16};
                    SDL_RenderDrawRect(app->renderer, &icon);
                }
                break;
            case TOOL_CIRCLE:
                /* Circle icon (approximated with lines) */
                for (int angle = 0; angle < 360; angle += 15) {
                    double rad1 = angle * 3.14159 / 180.0;
                    double rad2 = (angle + 15) * 3.14159 / 180.0;
                    int x1 = cx + (int)(10 * SDL_cos(rad1));
                    int y1 = cy + (int)(10 * SDL_sin(rad1));
                    int x2 = cx + (int)(10 * SDL_cos(rad2));
                    int y2 = cy + (int)(10 * SDL_sin(rad2));
                    SDL_RenderDrawLine(app->renderer, x1, y1, x2, y2);
                }
                break;
            case TOOL_FILL:
                /* Fill bucket icon - simple bucket shape */
                SDL_RenderDrawLine(app->renderer, cx - 8, cy - 8, cx + 8, cy - 8);
                SDL_RenderDrawLine(app->renderer, cx - 10, cy - 6, cx - 10, cy + 8);
                SDL_RenderDrawLine(app->renderer, cx + 10, cy - 6, cx + 10, cy + 8);
                SDL_RenderDrawLine(app->renderer, cx - 10, cy + 8, cx + 10, cy + 8);
                SDL_RenderDrawLine(app->renderer, cx - 8, cy - 8, cx - 10, cy - 6);
                SDL_RenderDrawLine(app->renderer, cx + 8, cy - 8, cx + 10, cy - 6);
                break;
        }
    }

    /* Draw color palette */
    for (int i = 0; i < (int)PALETTE_COUNT; i++) {
        draw_button(app, &app->color_buttons[i]);
    }

    /* Draw current color display */
    SDL_Rect color_display = app->current_color_display.rect;
    SDL_SetRenderDrawColor(app->renderer, app->current_color.r, app->current_color.g,
                           app->current_color.b, 255);
    SDL_RenderFillRect(app->renderer, &color_display);
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(app->renderer, &color_display);

    /* Draw status text area */
    SDL_Rect status_rect = {580, 10, 200, 40};
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(app->renderer, &status_rect);
    SDL_SetRenderDrawColor(app->renderer, 180, 180, 180, 255);
    SDL_RenderDrawRect(app->renderer, &status_rect);

    /* Draw keyboard shortcuts hint area */
    SDL_Rect hint_rect = {800, 10, 210, 40};
    SDL_SetRenderDrawColor(app->renderer, 250, 250, 220, 255);
    SDL_RenderFillRect(app->renderer, &hint_rect);
    SDL_SetRenderDrawColor(app->renderer, 180, 180, 180, 255);
    SDL_RenderDrawRect(app->renderer, &hint_rect);
}

/**
 * Render the application
 */
static void render(AppState *app) {
    /* Clear background */
    SDL_SetRenderDrawColor(app->renderer, 128, 128, 128, 255);
    SDL_RenderClear(app->renderer);

    /* Draw toolbar */
    draw_toolbar(app);

    /* Draw canvas background (checkerboard for transparency) */
    SDL_Rect canvas_bg = {app->canvas_offset_x - 2, app->canvas_offset_y - 2,
                          CANVAS_WIDTH + 4, CANVAS_HEIGHT + 4};
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(app->renderer, &canvas_bg);

    /* Draw canvas border */
    SDL_SetRenderDrawColor(app->renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(app->renderer, &canvas_bg);

    /* Draw canvas content */
    SDL_Rect canvas_rect = {app->canvas_offset_x, app->canvas_offset_y,
                            app->image->width, app->image->height};
    SDL_RenderCopy(app->renderer, app->canvas_texture, NULL, &canvas_rect);

    /* Draw preview for shape tools while dragging */
    if (app->is_drawing && app->current_tool != TOOL_PENCIL && app->current_tool != TOOL_FILL) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        int x = mx - app->canvas_offset_x;
        int y = my - app->canvas_offset_y;

        SDL_SetRenderDrawColor(app->renderer, app->current_color.r, app->current_color.g,
                               app->current_color.b, 200);

        switch (app->current_tool) {
            case TOOL_LINE:
                SDL_RenderDrawLine(app->renderer, app->start_x + app->canvas_offset_x,
                                   app->start_y + app->canvas_offset_y,
                                   mx, my);
                break;
            case TOOL_RECTANGLE:
                {
                    int rx = (app->start_x < x ? app->start_x : x) + app->canvas_offset_x;
                    int ry = (app->start_y < y ? app->start_y : y) + app->canvas_offset_y;
                    int rw = abs(x - app->start_x);
                    int rh = abs(y - app->start_y);
                    SDL_Rect preview = {rx, ry, rw, rh};
                    SDL_RenderDrawRect(app->renderer, &preview);
                }
                break;
            case TOOL_CIRCLE:
                {
                    int dx = x - app->start_x;
                    int dy = y - app->start_y;
                    int radius = (int)SDL_sqrt((double)(dx * dx + dy * dy));
                    int cx = app->start_x + app->canvas_offset_x;
                    int cy = app->start_y + app->canvas_offset_y;
                    for (int angle = 0; angle < 360; angle += 5) {
                        double rad1 = angle * 3.14159 / 180.0;
                        double rad2 = (angle + 5) * 3.14159 / 180.0;
                        int x1 = cx + (int)(radius * SDL_cos(rad1));
                        int y1 = cy + (int)(radius * SDL_sin(rad1));
                        int x2 = cx + (int)(radius * SDL_cos(rad2));
                        int y2 = cy + (int)(radius * SDL_sin(rad2));
                        SDL_RenderDrawLine(app->renderer, x1, y1, x2, y2);
                    }
                }
                break;
            default:
                break;
        }
    }

    SDL_RenderPresent(app->renderer);
}

/**
 * Main entry point
 */
int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    AppState app = {0};
    app.current_color = pixel_create(0, 0, 0);
    app.current_tool = TOOL_PENCIL;
    app.running = 1;

    if (!init_sdl(&app)) {
        cleanup_sdl(&app);
        return 1;
    }

    init_ui(&app);

    /* Create initial blank image */
    app.image = image_create(CANVAS_WIDTH, CANVAS_HEIGHT);
    if (!app.image) {
        fprintf(stderr, "Failed to create image\n");
        cleanup_sdl(&app);
        return 1;
    }

    /* Fill with white */
    Pixel white = {255, 255, 255};
    image_fill(app.image, white);
    update_canvas_texture(&app);

    printf("Graphics Editor started!\n");
    printf("Controls:\n");
    printf("  1-5: Select tools (Pencil, Line, Rectangle, Circle, Fill)\n");
    printf("  N: New image, S: Save, L: Load\n");
    printf("  G: Grayscale, I: Invert, R: Rotate, H/V: Flip\n");
    printf("  Delete/Backspace: Clear canvas\n");
    printf("  Escape: Exit\n");

    /* Main loop */
    while (app.running) {
        handle_events(&app);
        render(&app);
        SDL_Delay(16); /* ~60 FPS */
    }

    cleanup_sdl(&app);
    printf("Goodbye!\n");

    return 0;
}
