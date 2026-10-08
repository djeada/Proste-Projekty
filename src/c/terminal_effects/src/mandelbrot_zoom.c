// Zooms into the Mandelbrot set near the "seahorse valley".
#include <stdio.h>

#include "term.h"

static const uint32_t palette[16] = {
    0x000764, 0x02308c, 0x0a5bb4, 0x2085d2, 0x4cb0e6, 0x8bd6f2, 0xd2f0f7, 0xfff7c8,
    0xffe07a, 0xffb52e, 0xf4800c, 0xd94f07, 0xa82808, 0x7a1240, 0x4a0a6e, 0x1e0368,
};

int main(int argc, char **argv) {
    const double cx = -0.743643887037151, cy = 0.131825904205330;
    double scale = 3;
    int frame = 0;
    char status[64];
    start(argc, argv, 30);
    for (;;) {
        int max_iter = 60 + frame * 3;
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) {
                double re = cx + (x - W / 2.0) * scale / W;
                double im = cy + (y - H / 2.0) * scale / W;
                double zr = 0, zi = 0;
                int it = 0;
                while (zr * zr + zi * zi < 4 && it < max_iter) {
                    double t = zr * zr - zi * zi + re;
                    zi = 2 * zr * zi + im;
                    zr = t;
                    it++;
                }
                pixels[y][x] = it == max_iter ? 0 : palette[(it + frame) % 16];
            }
        snprintf(status, sizeof status, " zoom %dx, %d iterations", (int)(3 / scale), max_iter);
        show_pixels(status);
        scale *= 0.93;
        if (++frame == 130) {
            frame = 0;
            scale = 3;
        }
    }
}
