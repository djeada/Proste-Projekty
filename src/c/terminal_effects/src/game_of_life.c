// Conway's Game of Life on a wrapping grid, cells colored by age.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define W 24
#define H 25
#define GENERATIONS 230

static const int age_color[] = {231, 195, 159, 123, 87, 51, 45, 39, 33, 27, 21, 20};
static int grid[H][W], next[H][W];

static void drop_glider(void) {
    static const int g[3][3] = {{0, 1, 0}, {0, 0, 1}, {1, 1, 1}};
    int gx = rand() % W, gy = rand() % H;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (g[i][j]) grid[(gy + i) % H][(gx + j) % W] = 1;
}

int main(void) {
    srand(42);
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            grid[y][x] = rand() % 100 < 33;  // value = age, 0 is dead

    printf("\033[2J");
    for (int gen = 0; gen < GENERATIONS; gen++) {
        int alive = 0, last = -1;
        printf("\033[H");
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                int age = grid[y][x];
                if (!age) {
                    fputs("  ", stdout);
                    continue;
                }
                alive++;
                int c = age_color[age < 12 ? age - 1 : 11];
                if (c != last) {
                    printf("\033[38;5;%dm", c);
                    last = c;
                }
                fputs("██", stdout);
            }
            putchar('\n');
        }
        printf("\033[0m  generation %3d  |  alive %3d\n", gen, alive);
        fflush(stdout);

        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) {
                int n = 0;
                for (int dy = -1; dy <= 1; dy++)
                    for (int dx = -1; dx <= 1; dx++)
                        if ((dy || dx) && grid[(y + dy + H) % H][(x + dx + W) % W])
                            n++;
                int age = grid[y][x];
                next[y][x] = (n == 3 || (age && n == 2)) ? age + 1 : 0;
            }
        memcpy(grid, next, sizeof grid);
        if (gen % 25 == 24) drop_glider();  // keep the world lively
        usleep(35000);
    }
    return 0;
}
