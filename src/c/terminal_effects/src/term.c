#include "term.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint32_t pixels[H][W];
char text[ROWS][COLS];
uint32_t ink[ROWS][COLS];

static long frames_left;
static int delay;
static volatile sig_atomic_t stop;
static uint32_t state = 1;
static double last_frame;

static double now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static void on_signal(int sig) {
    (void)sig;
    stop = 1;
}

static void restore(void) {
    fputs("\033[0m\033[?25h\n", stdout);
    fflush(stdout);
}

void start(int argc, char **argv, int delay_ms) {
    frames_left = argc > 1 ? atol(argv[1]) : 0;
    delay = getenv("NO_SLEEP") ? 0 : delay_ms;
    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);
    atexit(restore);
    fputs("\033[?25l\033[2J", stdout);
    last_frame = now();
}

static void color(int layer, uint32_t c) {
    printf("\033[%d;2;%u;%u;%um", layer, c >> 16, c >> 8 & 255, c & 255);
}

static void end_frame(const char *status) {
    printf("\033[0m%s\033[K", status);
    fflush(stdout);
    double wait = delay / 1000.0 - (now() - last_frame);
    if (wait > 0) {
        struct timespec ts = {0, (long)(wait * 1e9)};
        nanosleep(&ts, NULL);
    }
    last_frame = now();
    if (stop || (frames_left > 0 && --frames_left == 0)) exit(0);
}

void show_pixels(const char *status) {
    fputs("\033[H", stdout);
    for (int y = 0; y < H; y += 2) {
        uint32_t fg = UINT32_MAX, bg = UINT32_MAX;
        for (int x = 0; x < W; x++) {
            if (pixels[y][x] != fg) color(38, fg = pixels[y][x]);
            if (pixels[y + 1][x] != bg) color(48, bg = pixels[y + 1][x]);
            fputs("▀", stdout);
        }
        fputs("\033[0m\n", stdout);
    }
    end_frame(status);
}

void show_text(const char *status) {
    fputs("\033[H", stdout);
    for (int y = 0; y < ROWS; y++) {
        uint32_t fg = UINT32_MAX;
        color(48, 0);
        for (int x = 0; x < COLS; x++) {
            if (text[y][x] != ' ' && ink[y][x] != fg) color(38, fg = ink[y][x]);
            putchar(text[y][x]);
        }
        fputs("\033[0m\n", stdout);
    }
    end_frame(status);
}

void seed(uint32_t s) { state = s; }

int rnd(int n) {
    state = state * 1103515245u + 12345u;
    return (int)(state >> 16) % n;
}
