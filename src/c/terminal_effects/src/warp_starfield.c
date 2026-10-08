// Flying through a 3D starfield: the faster we go, the longer the streaks.
#include <stdio.h>
#include <string.h>

#include "term.h"

#define STARS 300

static double sx[STARS], sy[STARS], sz[STARS];
static int bright[H][W];

static void spawn(int i, double z) {
    sx[i] = (rnd(2001) - 1000) / 1000.0;
    sy[i] = (rnd(2001) - 1000) / 1000.0;
    sz[i] = z;
}

static void put(double x, double y, double z, int b) {
    int px = W / 2 + (int)(x / z * 26), py = H / 2 + (int)(y / z * 26);
    if (px >= 0 && px < W && py >= 0 && py < H && b > bright[py][px]) bright[py][px] = b;
}

int main(int argc, char **argv) {
    char status[64];
    seed(42);
    for (int i = 0; i < STARS; i++) spawn(i, 0.05 + rnd(1000) / 1000.0);
    start(argc, argv, 30);
    for (int frame = 0;; frame = (frame + 1) % 300) {
        double p = frame < 150 ? frame / 150.0 : (300 - frame) / 150.0;
        double speed = 0.003 + 0.03 * p * p;
        int trail = (int)(speed * 300);
        memset(bright, 0, sizeof bright);
        for (int i = 0; i < STARS; i++) {
            sz[i] -= speed;
            if (sz[i] < 0.02) spawn(i, 1.05);
            int b = (int)((1.1 - sz[i]) * 230);
            for (int k = trail; k >= 1; k--) put(sx[i], sy[i], sz[i] + k * speed * 0.6, b / (k + 1));
            put(sx[i], sy[i], sz[i], b);
        }
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) {
                uint32_t b = (uint32_t)bright[y][x];
                pixels[y][x] = (b * 3 / 4) << 16 | (b * 7 / 8) << 8 | b;
            }
        snprintf(status, sizeof status, " warp factor %d", 1 + (int)(p * 8));
        show_pixels(status);
    }
}
