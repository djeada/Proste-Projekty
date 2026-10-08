// A cube spinning around three axes. A z-buffer keeps the nearest face on top,
// and faces turned towards the viewer are lit brighter.
#include <math.h>
#include <string.h>

#include "term.h"

typedef struct {
    double x, y, z;
} vec;

static const uint32_t face_color[6] = {0xff4060, 0x40e070, 0x4080ff, 0xffd040, 0xe050ff, 0x40e0ff};
static const double normal[6][3] = {{0, 0, -1}, {1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, -1, 0}, {0, 1, 0}};

static double zbuf[H][W];
static double sA, cA, sB, cB, sC, cC;

static vec rotate(double i, double j, double k) {
    return (vec){j * sA * sB * cC - k * cA * sB * cC + j * cA * sC + k * sA * sC + i * cB * cC,
                 j * cA * cC + k * sA * cC - j * sA * sB * sC + k * cA * sB * sC - i * cB * sC,
                 k * cA * cB - j * sA * cB + i * sB};
}

static void plot(double i, double j, double k, uint32_t color) {
    vec p = rotate(i, j, k);
    double ooz = 1 / (p.z + 60);
    int px = W / 2 + (int)(60 * ooz * p.x), py = H / 2 + (int)(60 * ooz * p.y);
    if (px >= 0 && px < W && py >= 0 && py < H && ooz > zbuf[py][px]) {
        zbuf[py][px] = ooz;
        pixels[py][px] = color;
    }
}

static uint32_t lit(int face) {
    double light = 0.3 + 0.7 * fmax(0, -rotate(normal[face][0], normal[face][1], normal[face][2]).z);
    uint32_t c = face_color[face], out = 0;
    for (int s = 0; s <= 16; s += 8) out |= (uint32_t)((c >> s & 255) * light) << s;
    return out;
}

int main(int argc, char **argv) {
    double A = 0, B = 0, C = 0;
    start(argc, argv, 30);
    for (;;) {
        sA = sin(A), cA = cos(A), sB = sin(B), cB = cos(B), sC = sin(C), cC = cos(C);
        memset(zbuf, 0, sizeof zbuf);
        memset(pixels, 0, sizeof pixels);
        uint32_t color[6];
        for (int f = 0; f < 6; f++) color[f] = lit(f);
        for (int u = 0; u < 40; u++)
            for (int v = 0; v < 40; v++) {
                double a = -10 + u * 0.5, b = -10 + v * 0.5;
                plot(a, b, -10, color[0]);
                plot(10, b, a, color[1]);
                plot(-10, b, -a, color[2]);
                plot(-a, b, 10, color[3]);
                plot(a, -10, -b, color[4]);
                plot(a, 10, b, color[5]);
            }
        show_pixels("");
        A += 0.05, B += 0.05, C += 0.01;
    }
}
