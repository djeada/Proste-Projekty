/* The SDL2 window: the canvas (shown at 2x), a tool panel and keyboard shortcuts. */
#include <SDL.h>
#include <stdio.h>

#include "graphics_editor.h"

#define CANVAS_W 320
#define CANVAS_H 240
#define SCALE 2
#define PANEL_X (CANVAS_W * SCALE)
#define PANEL_W 120
#define WINDOW_W (PANEL_X + PANEL_W)
#define WINDOW_H (CANVAS_H * SCALE)
#define DEFAULT_FILE "drawing.bmp"

enum Tool { TOOL_BRUSH, TOOL_ERASER, TOOL_LINE, TOOL_RECT, TOOL_FILL, TOOL_PICKER, TOOL_COUNT };

typedef struct {
    Canvas canvas;
    Canvas preview; /* the canvas with the line or rectangle being dragged drawn on it */
    History history;
    enum Tool tool;
    int width; /* brush width in pixels: 1, 3 or 5 */
    Color color;
    int drawing;
    int last_x;
    int last_y;
    int start_x;
    int start_y;
    const char *path;
} Editor;

static const Color WHITE = {255, 255, 255};
static const Color PALETTE[12] = {
    {0, 0, 0},       {255, 255, 255}, {128, 128, 128}, {160, 0, 0},
    {255, 0, 0},     {255, 128, 0},   {255, 230, 0},   {0, 160, 0},
    {0, 220, 120},   {0, 120, 255},   {120, 0, 255},   {255, 0, 200},
};
static const int BRUSH_WIDTHS[3] = {1, 3, 5};

/* Panel layout, relative to the window. */
static SDL_Rect tool_button(int index)
{
    SDL_Rect rect = {PANEL_X + 4 + (index % 2) * 56, 8 + (index / 2) * 42, 52, 36};
    return rect;
}

static SDL_Rect size_button(int index)
{
    SDL_Rect rect = {PANEL_X + 4 + index * 38, 140, 32, 32};
    return rect;
}

static SDL_Rect swatch_button(int index)
{
    SDL_Rect rect = {PANEL_X + 4 + (index % 3) * 38, 190 + (index / 3) * 34, 30, 26};
    return rect;
}

static SDL_Rect clear_button(void)
{
    SDL_Rect rect = {PANEL_X + 4, 340, 112, 32};
    return rect;
}

static void to_pixels(const Canvas *canvas, void *pixels, int pitch)
{
    for (int y = 0; y < canvas->height; y++) {
        uint32_t *row = (uint32_t *)((uint8_t *)pixels + y * pitch);
        for (int x = 0; x < canvas->width; x++) {
            Color c = canvas->pixels[y * canvas->width + x];
            row[x] = ((uint32_t)c.r << 16) | ((uint32_t)c.g << 8) | c.b;
        }
    }
}

static void save_bmp(const Editor *editor)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, CANVAS_W, CANVAS_H, 32, 0x00FF0000,
                                                0x0000FF00, 0x000000FF, 0);
    if (surface == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return;
    }
    SDL_LockSurface(surface);
    to_pixels(&editor->canvas, surface->pixels, surface->pitch);
    SDL_UnlockSurface(surface);
    if (SDL_SaveBMP(surface, editor->path) != 0) {
        fprintf(stderr, "Cannot save %s: %s\n", editor->path, SDL_GetError());
    }
    SDL_FreeSurface(surface);
}

/* Missing or broken files are reported and the canvas is left as it was. */
static void open_bmp(Editor *editor)
{
    SDL_Surface *loaded = SDL_LoadBMP(editor->path);
    if (loaded == NULL) {
        fprintf(stderr, "Cannot open %s: %s\n", editor->path, SDL_GetError());
        return;
    }
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(loaded);
    if (surface == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return;
    }
    history_push(&editor->history, &editor->canvas);
    canvas_clear(&editor->canvas, WHITE);
    SDL_LockSurface(surface);
    for (int y = 0; y < surface->h && y < CANVAS_H; y++) {
        uint32_t *row = (uint32_t *)((uint8_t *)surface->pixels + y * surface->pitch);
        for (int x = 0; x < surface->w && x < CANVAS_W; x++) {
            Color c = {(uint8_t)(row[x] >> 16), (uint8_t)(row[x] >> 8), (uint8_t)row[x]};
            canvas_set(&editor->canvas, x, y, c);
        }
    }
    SDL_UnlockSurface(surface);
    SDL_FreeSurface(surface);
}

static int is_shape_tool(enum Tool tool)
{
    return tool == TOOL_LINE || tool == TOOL_RECT;
}

static void draw_shape(Canvas *canvas, enum Tool tool, const Editor *editor, int x, int y)
{
    if (tool == TOOL_LINE) {
        draw_line(canvas, editor->start_x, editor->start_y, x, y, editor->width, editor->color);
    } else {
        draw_rect(canvas, editor->start_x, editor->start_y, x, y, editor->width, editor->color);
    }
}

static Color stroke_color(const Editor *editor)
{
    return editor->tool == TOOL_ERASER ? WHITE : editor->color;
}

static void press_canvas(Editor *editor, int x, int y)
{
    if (editor->tool == TOOL_PICKER) {
        if (canvas_inside(&editor->canvas, x, y)) {
            editor->color = canvas_get(&editor->canvas, x, y);
        }
        return;
    }
    history_push(&editor->history, &editor->canvas);
    editor->drawing = 1;
    editor->start_x = editor->last_x = x;
    editor->start_y = editor->last_y = y;
    if (editor->tool == TOOL_FILL) {
        flood_fill(&editor->canvas, x, y, editor->color);
        editor->drawing = 0;
    } else if (editor->tool == TOOL_BRUSH || editor->tool == TOOL_ERASER) {
        draw_line(&editor->canvas, x, y, x, y, editor->width, stroke_color(editor));
    } else {
        canvas_copy_into(&editor->preview, &editor->canvas);
    }
}

static void move_canvas(Editor *editor, int x, int y)
{
    if (!editor->drawing) {
        return;
    }
    if (editor->tool == TOOL_BRUSH || editor->tool == TOOL_ERASER) {
        draw_line(&editor->canvas, editor->last_x, editor->last_y, x, y, editor->width,
                  stroke_color(editor));
    } else {
        canvas_copy_into(&editor->preview, &editor->canvas);
        draw_shape(&editor->preview, editor->tool, editor, x, y);
    }
    editor->last_x = x;
    editor->last_y = y;
}

static void release_canvas(Editor *editor)
{
    if (editor->drawing && is_shape_tool(editor->tool)) {
        draw_shape(&editor->canvas, editor->tool, editor, editor->last_x, editor->last_y);
    }
    editor->drawing = 0;
}

static void press_panel(Editor *editor, int x, int y)
{
    SDL_Point point = {x, y};
    for (int i = 0; i < TOOL_COUNT; i++) {
        SDL_Rect rect = tool_button(i);
        if (SDL_PointInRect(&point, &rect)) {
            editor->tool = (enum Tool)i;
        }
    }
    for (int i = 0; i < 3; i++) {
        SDL_Rect rect = size_button(i);
        if (SDL_PointInRect(&point, &rect)) {
            editor->width = BRUSH_WIDTHS[i];
        }
    }
    for (int i = 0; i < 12; i++) {
        SDL_Rect rect = swatch_button(i);
        if (SDL_PointInRect(&point, &rect)) {
            editor->color = PALETTE[i];
        }
    }
    SDL_Rect clear = clear_button();
    if (SDL_PointInRect(&point, &clear)) {
        history_push(&editor->history, &editor->canvas);
        canvas_clear(&editor->canvas, WHITE);
    }
}

/* Ctrl+key shortcuts and single-key tool shortcuts. */
static void press_key(Editor *editor, SDL_Keycode key, int ctrl)
{
    if (ctrl) {
        if (key == SDLK_z) {
            history_undo(&editor->history, &editor->canvas);
        } else if (key == SDLK_s) {
            save_bmp(editor);
        } else if (key == SDLK_o) {
            open_bmp(editor);
        }
        return;
    }
    switch (key) {
    case SDLK_b: editor->tool = TOOL_BRUSH; break;
    case SDLK_e: editor->tool = TOOL_ERASER; break;
    case SDLK_l: editor->tool = TOOL_LINE; break;
    case SDLK_r: editor->tool = TOOL_RECT; break;
    case SDLK_f: editor->tool = TOOL_FILL; break;
    case SDLK_p: editor->tool = TOOL_PICKER; break;
    case SDLK_1: editor->width = BRUSH_WIDTHS[0]; break;
    case SDLK_2: editor->width = BRUSH_WIDTHS[1]; break;
    case SDLK_3: editor->width = BRUSH_WIDTHS[2]; break;
    case SDLK_n:
        history_push(&editor->history, &editor->canvas);
        canvas_clear(&editor->canvas, WHITE);
        break;
    default: break;
    }
}

static void draw_outline(SDL_Renderer *renderer, SDL_Rect rect, int r, int g, int b)
{
    SDL_SetRenderDrawColor(renderer, (Uint8)r, (Uint8)g, (Uint8)b, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

static void draw_button(SDL_Renderer *renderer, SDL_Rect rect, int selected)
{
    SDL_SetRenderDrawColor(renderer, selected ? 255 : 200, selected ? 220 : 200,
                           selected ? 120 : 200, 255);
    SDL_RenderFillRect(renderer, &rect);
    draw_outline(renderer, rect, 60, 60, 60);
}

/* Each tool icon is drawn from a few rectangles and lines inside its button. */
static void draw_tool_icon(SDL_Renderer *renderer, SDL_Rect r, enum Tool tool)
{
    int x = r.x + 10, y = r.y + 6, w = r.w - 20, h = r.h - 12;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    switch (tool) {
    case TOOL_BRUSH:
        for (int d = 0; d < 3; d++) {
            SDL_RenderDrawLine(renderer, x + d, y + h, x + w + d, y + d);
        }
        break;
    case TOOL_ERASER: {
        SDL_Rect eraser = {x, y, w, h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &eraser);
        draw_outline(renderer, eraser, 0, 0, 0);
        break;
    }
    case TOOL_LINE:
        SDL_RenderDrawLine(renderer, x, y + h, x + w, y);
        break;
    case TOOL_RECT:
        draw_outline(renderer, (SDL_Rect){x, y, w, h}, 0, 0, 0);
        break;
    case TOOL_FILL: {
        SDL_Rect fill = {x, y + h / 2, w, h / 2};
        SDL_RenderFillRect(renderer, &fill);
        SDL_RenderDrawLine(renderer, x + w / 2, y, x + w / 2, y + h / 2);
        break;
    }
    case TOOL_PICKER:
        SDL_RenderFillRect(renderer, &(SDL_Rect){x, y + h - 4, 5, 5});
        SDL_RenderDrawLine(renderer, x + 4, y + h - 4, x + w, y);
        break;
    default:
        break;
    }
}

static void draw_panel(SDL_Renderer *renderer, const Editor *editor)
{
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_Rect panel = {PANEL_X, 0, PANEL_W, WINDOW_H};
    SDL_RenderFillRect(renderer, &panel);

    for (int i = 0; i < TOOL_COUNT; i++) {
        SDL_Rect rect = tool_button(i);
        draw_button(renderer, rect, editor->tool == (enum Tool)i);
        draw_tool_icon(renderer, rect, (enum Tool)i);
    }
    for (int i = 0; i < 3; i++) {
        SDL_Rect rect = size_button(i);
        draw_button(renderer, rect, editor->width == BRUSH_WIDTHS[i]);
        int side = 3 + 4 * i;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect dot = {rect.x + (rect.w - side) / 2, rect.y + (rect.h - side) / 2, side, side};
        SDL_RenderFillRect(renderer, &dot);
    }
    for (int i = 0; i < 12; i++) {
        SDL_Rect rect = swatch_button(i);
        Color c = PALETTE[i];
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
        SDL_RenderFillRect(renderer, &rect);
        int selected = c.r == editor->color.r && c.g == editor->color.g && c.b == editor->color.b;
        draw_outline(renderer, rect, selected ? 0 : 90, selected ? 0 : 90, selected ? 0 : 90);
    }
    SDL_Rect clear = clear_button();
    draw_button(renderer, clear, 0);
    SDL_SetRenderDrawColor(renderer, 160, 0, 0, 255);
    int cx = clear.x + clear.w / 2, cy = clear.y + clear.h / 2;
    SDL_RenderDrawLine(renderer, cx - 8, cy - 8, cx + 8, cy + 8);
    SDL_RenderDrawLine(renderer, cx + 8, cy - 8, cx - 8, cy + 8);
}

static void render(SDL_Renderer *renderer, SDL_Texture *texture, const Editor *editor)
{
    const Canvas *view = editor->drawing && is_shape_tool(editor->tool) ? &editor->preview
                                                                        : &editor->canvas;
    void *pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &pixels, &pitch);
    to_pixels(view, pixels, pitch);
    SDL_UnlockTexture(texture);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderClear(renderer);
    SDL_Rect canvas_rect = {0, 0, CANVAS_W * SCALE, CANVAS_H * SCALE};
    SDL_RenderCopy(renderer, texture, NULL, &canvas_rect);
    draw_panel(renderer, editor);
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_Window *window = SDL_CreateWindow("Graphics editor", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
                                             SDL_TEXTUREACCESS_STREAMING, CANVAS_W, CANVAS_H);

    Editor editor = {0};
    editor.canvas = canvas_create(CANVAS_W, CANVAS_H, WHITE);
    editor.preview = canvas_create(CANVAS_W, CANVAS_H, WHITE);
    history_init(&editor.history);
    editor.tool = TOOL_BRUSH;
    editor.width = BRUSH_WIDTHS[0];
    editor.color = PALETTE[0];
    editor.path = argc > 1 ? argv[1] : DEFAULT_FILE;
    if (argc > 1) {
        open_bmp(&editor);
    }

    int running = 1;
    SDL_Event event;
    render(renderer, texture, &editor);
    while (running && SDL_WaitEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button != SDL_BUTTON_LEFT) {
                break;
            }
            if (event.button.x >= PANEL_X) {
                press_panel(&editor, event.button.x, event.button.y);
            } else {
                press_canvas(&editor, event.button.x / SCALE, event.button.y / SCALE);
            }
            break;
        case SDL_MOUSEMOTION:
            move_canvas(&editor, event.motion.x / SCALE, event.motion.y / SCALE);
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                release_canvas(&editor);
            }
            break;
        case SDL_KEYDOWN:
            press_key(&editor, event.key.keysym.sym, event.key.keysym.mod & KMOD_CTRL);
            break;
        default:
            break;
        }
        render(renderer, texture, &editor);
    }

    history_free(&editor.history);
    canvas_destroy(&editor.canvas);
    canvas_destroy(&editor.preview);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
