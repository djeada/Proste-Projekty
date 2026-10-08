// Conway's Game of Life on a wrapping board, cells colored by age.
#include <stdio.h>
#include <string.h>

#include "term.h"

static const uint32_t age_color[8] = {
    0xffffff, 0xc8fff4, 0x8ef5e6, 0x4fd8e0, 0x2aa8d8, 0x2a72c8, 0x3044a8, 0x2a2a80,
};

static int grid[H][W], next[H][W];

static void drop_glider(void) {
    static const int glider[5][2] = {{0, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}};
    int gx = rnd(W), gy = rnd(H);
    for (int i = 0; i < 5; i++) grid[(gy + glider[i][0]) % H][(gx + glider[i][1]) % W] = 1;
}

int main(int argc, char **argv) {
    char status[64];
    seed(42);
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) grid[y][x] = rnd(3) == 0;
    start(argc, argv, 40);
    for (int gen = 0;; gen++) {
        int alive = 0;
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) {
                int age = grid[y][x];
                alive += age > 0;
                pixels[y][x] = age ? age_color[age < 8 ? age - 1 : 7] : 0;
            }
        snprintf(status, sizeof status, " generation %d, alive %d", gen, alive);
        show_pixels(status);

        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) {
                int n = 0;
                for (int dy = -1; dy <= 1; dy++)
                    for (int dx = -1; dx <= 1; dx++)
                        if ((dy || dx) && grid[(y + dy + H) % H][(x + dx + W) % W]) n++;
                int age = grid[y][x];
                next[y][x] = n == 3 || (age && n == 2) ? age + 1 : 0;
            }
        memcpy(grid, next, sizeof grid);
        if (gen % 20 == 19) drop_glider();
    }
}
