// Carves a random maze with depth-first search, then finds the shortest path with BFS.
#include <stdio.h>

#include "term.h"

#define MW (W - 1)
#define MH (H - 1)
#define WALL (-2)
#define OPEN (-1)

static const uint32_t ripple[8] = {0x20e0ff, 0x2098ff, 0x3060ff, 0x6040f0, 0x9030e0, 0x6040f0, 0x3060ff, 0x2098ff};

static int maze[MH][MW], from[MH][MW], on_path[MH][MW];

static uint32_t color(int x, int y, int hx, int hy) {
    if (x == 1 && y == 1) return 0x40ff70;
    if (x == MW - 2 && y == MH - 2) return 0xff4040;
    if (x == hx && y == hy) return 0xff40ff;
    if (on_path[y][x]) return 0xffe040;
    if (maze[y][x] == WALL) return 0x384050;
    if (maze[y][x] >= 0) return ripple[maze[y][x] / 4 % 8];
    return 0;
}

static void draw(int hx, int hy, const char *status) {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) pixels[y][x] = x < MW && y < MH ? color(x, y, hx, hy) : 0;
    show_pixels(status);
}

static void carve(void) {
    static int stack[MW * MH];
    int top = 0, dirs[4][2] = {{0, -2}, {2, 0}, {0, 2}, {-2, 0}};
    maze[1][1] = OPEN;
    stack[top++] = 1 * MW + 1;
    while (top) {
        int x = stack[top - 1] % MW, y = stack[top - 1] / MW, moved = 0;
        for (int i = 3; i > 0; i--) {
            int j = rnd(i + 1), dx = dirs[i][0], dy = dirs[i][1];
            dirs[i][0] = dirs[j][0], dirs[i][1] = dirs[j][1];
            dirs[j][0] = dx, dirs[j][1] = dy;
        }
        for (int i = 0; i < 4 && !moved; i++) {
            int nx = x + dirs[i][0], ny = y + dirs[i][1];
            if (nx > 0 && nx < MW - 1 && ny > 0 && ny < MH - 1 && maze[ny][nx] == WALL) {
                maze[y + dirs[i][1] / 2][x + dirs[i][0] / 2] = OPEN;
                maze[ny][nx] = OPEN;
                stack[top++] = ny * MW + nx;
                moved = 1;
                draw(nx, ny, " carving the maze (depth-first search)");
            }
        }
        if (!moved) top--;
    }
}

static void solve(void) {
    static const int dirs[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    static int queue[MW * MH];
    int head = 0, tail = 0, layer = 0, goal = (MH - 2) * MW + MW - 2;
    char status[64];
    maze[1][1] = 0;
    queue[tail++] = 1 * MW + 1;
    while (head < tail) {
        int cell = queue[head++], x = cell % MW, y = cell / MW;
        if (maze[y][x] > layer) {
            layer = maze[y][x];
            snprintf(status, sizeof status, " searching (breadth-first search): distance %d", layer);
            if (layer % 2 == 0) draw(-1, -1, status);
        }
        if (cell == goal) break;
        for (int i = 0; i < 4; i++) {
            int nx = x + dirs[i][0], ny = y + dirs[i][1];
            if (maze[ny][nx] == OPEN) {
                maze[ny][nx] = maze[y][x] + 1;
                from[ny][nx] = cell;
                queue[tail++] = ny * MW + nx;
            }
        }
    }
    snprintf(status, sizeof status, " shortest path: %d steps", maze[MH - 2][MW - 2]);
    for (int cell = goal; cell != 1 * MW + 1; cell = from[cell / MW][cell % MW]) {
        on_path[cell / MW][cell % MW] = 1;
        draw(-1, -1, status);
    }
    for (int k = 0; k < 60; k++) draw(-1, -1, status);
}

int main(int argc, char **argv) {
    seed(2024);
    start(argc, argv, 15);
    for (;;) {
        for (int y = 0; y < MH; y++)
            for (int x = 0; x < MW; x++) maze[y][x] = WALL, on_path[y][x] = 0;
        carve();
        solve();
    }
}
