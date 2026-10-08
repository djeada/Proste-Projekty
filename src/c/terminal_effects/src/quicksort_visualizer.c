// Quicksort, one frame per swap. White bars are being swapped, magenta is the pivot.
#include <stdio.h>

#include "term.h"

static const uint32_t keys[5] = {0xff3030, 0xffd030, 0x30e060, 0x30c0ff, 0xa040ff};

static int a[W], compares, swaps;

static uint32_t rainbow(int v) {
    int p = (v - 1) * 1024 / H, f = p % 256;
    uint32_t c1 = keys[p / 256], c2 = keys[p / 256 + 1], c = 0;
    for (int s = 0; s <= 16; s += 8) c |= ((c1 >> s & 255) * (256 - f) + (c2 >> s & 255) * f) / 256 << s;
    return c;
}

static void draw(int i1, int i2, int pivot, int done) {
    char status[64];
    for (int x = 0; x < W; x++) {
        uint32_t c = x < done ? 0x40ff70 : x == pivot ? 0xff40ff : x == i1 || x == i2 ? 0xffffff : rainbow(a[x]);
        for (int y = 0; y < H; y++) pixels[y][x] = y >= H - a[x] ? c : 0;
    }
    snprintf(status, sizeof status, " quicksort: %d comparisons, %d swaps", compares, swaps);
    show_pixels(status);
}

static void swap(int i, int j, int pivot) {
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
    swaps++;
    draw(i, j, pivot, 0);
}

static void quicksort(int lo, int hi) {
    if (lo >= hi) return;
    int pivot = a[hi], i = lo;
    for (int j = lo; j < hi; j++) {
        compares++;
        if (a[j] < pivot) swap(i++, j, hi);
    }
    swap(i, hi, i);
    quicksort(lo, i - 1);
    quicksort(i + 1, hi);
}

int main(int argc, char **argv) {
    seed(7);
    for (int i = 0; i < W; i++) a[i] = 1 + i * H / W;
    start(argc, argv, 30);
    for (;;) {
        for (int i = W - 1; i > 0; i--) {
            int j = rnd(i + 1), t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
        compares = swaps = 0;
        for (int k = 0; k < 20; k++) draw(-1, -1, -1, 0);
        quicksort(0, W - 1);
        for (int done = 1; done <= W + 40; done++) draw(-1, -1, -1, done);
    }
}
