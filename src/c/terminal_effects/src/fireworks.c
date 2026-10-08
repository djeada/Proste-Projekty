// Fireworks: rockets fly up, burst into sparks, and gravity pulls everything down.
// Old frames are dimmed instead of cleared, which leaves glowing trails.
#include "term.h"

#define MAX 400

static const uint32_t colors[6] = {0xff5050, 0xffd040, 0x50ff80, 0x50c8ff, 0xd070ff, 0xffffff};

static double px[MAX], py[MAX], vx[MAX], vy[MAX];
static int life[MAX], rocket[MAX];
static uint32_t color[MAX];

static int spawn(double x, double y, double dx, double dy, int frames, uint32_t c) {
    for (int i = 0; i < MAX; i++)
        if (!life[i]) {
            px[i] = x, py[i] = y, vx[i] = dx, vy[i] = dy, life[i] = frames, color[i] = c, rocket[i] = 0;
            return i;
        }
    return -1;
}

static void burst(double x, double y) {
    uint32_t c = colors[rnd(6)];
    for (int n = 0; n < 60; n++) {
        double dx, dy;
        do {
            dx = (rnd(201) - 100) / 100.0;
            dy = (rnd(201) - 100) / 100.0;
        } while (dx * dx + dy * dy > 1);
        spawn(x, y, dx, dy, 25 + rnd(20), c);
    }
}

static uint32_t dim(uint32_t c, int num, int den) {
    return ((c >> 16) * num / den) << 16 | ((c >> 8 & 255) * num / den) << 8 | (c & 255) * num / den;
}

int main(int argc, char **argv) {
    seed(2025);
    start(argc, argv, 30);
    for (;;) {
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) pixels[y][x] = dim(pixels[y][x], 3, 4);
        if (rnd(12) == 0) {
            double x = 8 + rnd(W - 16), dx = (rnd(41) - 20) / 100.0, dy = -(1.3 + rnd(40) / 100.0);
            int i = spawn(x, H - 1, dx, dy, 100, 0xffe0b0);
            if (i >= 0) rocket[i] = 1;
        }
        for (int i = 0; i < MAX; i++) {
            if (!life[i]) continue;
            px[i] += vx[i];
            py[i] += vy[i];
            vy[i] += rocket[i] ? 0.05 : 0.02;
            vx[i] *= 0.97;
            life[i]--;
            if (rocket[i] && vy[i] >= 0) {
                life[i] = 0;
                burst(px[i], py[i]);
                continue;
            }
            int x = (int)px[i], y = (int)py[i];
            if (x >= 0 && x < W && y >= 0 && y < H)
                pixels[y][x] = rocket[i] ? color[i] : dim(color[i], life[i] + 15, 60);
        }
        show_pixels("");
    }
}
