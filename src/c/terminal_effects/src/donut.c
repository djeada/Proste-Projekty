// Spinning 3D torus in pure C: no libraries, just math.
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define W 50
#define H 24
#define FRAMES 170

static const char *ramp = ".,-~:;=!*#$@";
static const int glow[] = {22, 28, 34, 40, 46, 82, 118, 120, 157, 194, 231, 231};

int main(void) {
    float A = 0, B = 0, zbuf[W * H];
    char screen[W * H];
    int light[W * H];

    printf("\033[1;32m[*] spawning torus...\033[0m\n");
    fflush(stdout);
    usleep(600000);
    printf("\033[2J");

    for (int frame = 0; frame < FRAMES; frame++) {
        memset(screen, ' ', sizeof screen);
        memset(zbuf, 0, sizeof zbuf);
        for (float j = 0; j < 6.28f; j += 0.07f) {
            for (float i = 0; i < 6.28f; i += 0.02f) {
                float si = sinf(i), ci = cosf(i), sj = sinf(j), cj = cosf(j);
                float sA = sinf(A), cA = cosf(A), sB = sinf(B), cB = cosf(B);
                float h = cj + 2;
                float D = 1 / (si * h * sA + sj * cA + 5);
                float t = si * h * cA - sj * sA;
                int x = W / 2 + (int)(30 * D * (ci * h * cB - t * sB));
                int y = H / 2 + (int)(13 * D * (ci * h * sB + t * cB));
                int N = (int)(8 * ((sj * sA - si * cj * cA) * cB
                                   - si * cj * sA - sj * cA - ci * cj * sB));
                int o = x + W * y;
                if (y >= 0 && y < H && x >= 0 && x < W - 1 && D > zbuf[o]) {
                    zbuf[o] = D;
                    light[o] = N > 0 ? N : 0;
                    screen[o] = ramp[light[o]];
                }
            }
        }
        printf("\033[H");
        for (int k = 0; k < W * H; k++) {
            if (k % W == W - 1)
                putchar('\n');
            else if (screen[k] == ' ')
                putchar(' ');
            else
                printf("\033[38;5;%dm%c", glow[light[k]], screen[k]);
        }
        printf("\033[0m");
        fflush(stdout);
        A += 0.07f;
        B += 0.03f;
        usleep(33000);
    }
    printf("\033[1;32m[+] %d frames rendered. no GPU needed.\033[0m\n", FRAMES);
    return 0;
}
