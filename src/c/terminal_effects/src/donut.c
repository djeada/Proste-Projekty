// A spinning torus drawn with ASCII characters: brighter character, more light.
#include <math.h>
#include <string.h>

#include "term.h"

static const char *ramp = ".,-~:;=!*#$@";

int main(int argc, char **argv) {
    double A = 0, B = 0, zbuf[ROWS][COLS];
    start(argc, argv, 30);
    for (;;) {
        memset(text, ' ', sizeof text);
        memset(zbuf, 0, sizeof zbuf);
        double sA = sin(A), cA = cos(A), sB = sin(B), cB = cos(B);
        for (int j = 0; j < 90; j++) {
            double sj = sin(j * 0.07), cj = cos(j * 0.07), h = cj + 2;
            for (int i = 0; i < 314; i++) {
                double si = sin(i * 0.02), ci = cos(i * 0.02);
                double D = 1 / (si * h * sA + sj * cA + 5);
                double t = si * h * cA - sj * sA;
                int x = COLS / 2 + (int)(32 * D * (ci * h * cB - t * sB));
                int y = ROWS / 2 + (int)(14 * D * (ci * h * sB + t * cB));
                int L = (int)(8 * ((sj * sA - si * cj * cA) * cB - si * cj * sA - sj * cA - ci * cj * sB));
                if (L < 0) L = 0;
                if (x >= 0 && x < COLS && y >= 0 && y < ROWS && D > zbuf[y][x]) {
                    zbuf[y][x] = D;
                    text[y][x] = ramp[L];
                    ink[y][x] = (uint32_t)(L * 16 << 16 | (80 + L * 15) << 8 | L * 12);
                }
            }
        }
        show_text("");
        A += 0.07;
        B += 0.03;
    }
}
