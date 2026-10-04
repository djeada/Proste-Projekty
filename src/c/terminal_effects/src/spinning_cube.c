// A spinning cube rasterized with a z-buffer, one color per face.
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define W 48
#define H 26
#define FRAMES 200

static float sA, cA, sB, cB, sC, cC;
static float zbuf[W * H];
static int cell[W * H];

static void plot(float i, float j, float k, int color) {
    // rotate the point around the x, y and z axes
    float x = j * sA * sB * cC - k * cA * sB * cC + j * cA * sC + k * sA * sC + i * cB * cC;
    float y = j * cA * cC + k * sA * cC - j * sA * sB * sC + k * cA * sB * sC - i * cB * sC;
    float z = k * cA * cB - j * sA * cB + i * sB + 60;
    float ooz = 1 / z;
    int xp = (int)(W / 2 + 24 * ooz * x * 2.2f);
    int yp = (int)(H / 2 + 24 * ooz * y);
    int idx = xp + yp * W;
    if (xp >= 0 && xp < W && yp >= 0 && yp < H && ooz > zbuf[idx]) {
        zbuf[idx] = ooz;
        cell[idx] = color;
    }
}

int main(void) {
    const float s = 10;
    float A = 0, B = 0, C = 0;
    printf("\033[2J");
    for (int frame = 0; frame < FRAMES; frame++) {
        sA = sinf(A), cA = cosf(A), sB = sinf(B), cB = cosf(B), sC = sinf(C), cC = cosf(C);
        memset(zbuf, 0, sizeof zbuf);
        memset(cell, 0, sizeof cell);
        for (float a = -s; a < s; a += 0.25f)
            for (float b = -s; b < s; b += 0.25f) {
                plot(a, b, -s, 196);   // front: red
                plot(s, b, a, 46);     // right: green
                plot(-s, b, -a, 33);   // left: blue
                plot(-a, b, s, 226);   // back: yellow
                plot(a, -s, -b, 201);  // bottom: magenta
                plot(a, s, b, 51);     // top: cyan
            }

        int last = -1;
        printf("\033[H");
        for (int k = 0; k < W * H; k++) {
            if (!cell[k]) {
                putchar(' ');
            } else {
                if (cell[k] != last) {
                    printf("\033[38;5;%dm", cell[k]);
                    last = cell[k];
                }
                fputs("█", stdout);
            }
            if (k % W == W - 1) putchar('\n');
        }
        printf("\033[0m");
        fflush(stdout);
        A += 0.05f, B += 0.05f, C += 0.01f;
        usleep(30000);
    }
    return 0;
}
