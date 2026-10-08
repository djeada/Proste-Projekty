// Langton's ant: on an empty cell turn right, on a filled cell turn left,
// flip the cell and step forward. After ~10,000 steps of chaos it builds a "highway".
#include <stdio.h>
#include <string.h>

#include "term.h"

static int cell[H][W];

int main(int argc, char **argv) {
    static const int dx[4] = {0, 1, 0, -1}, dy[4] = {-1, 0, 1, 0};
    char status[64];
    start(argc, argv, 30);
    for (;;) {
        int x = W / 2, y = H / 2, dir = 0;
        memset(cell, 0, sizeof cell);
        for (int step = 0; step < 12000; step++) {
            dir = (dir + (cell[y][x] ? 3 : 1)) % 4;
            cell[y][x] = !cell[y][x];
            x = (x + dx[dir] + W) % W;
            y = (y + dy[dir] + H) % H;
            if (step % 60 == 0) {
                for (int py = 0; py < H; py++)
                    for (int px = 0; px < W; px++) pixels[py][px] = cell[py][px] ? 0x30c0a0 : 0;
                pixels[y][x] = 0xff3050;
                snprintf(status, sizeof status, " step %d", step);
                show_pixels(status);
            }
        }
    }
}
