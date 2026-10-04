// The PSX Doom fire: heat rises, drifts sideways and cools at random.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define W 48
#define H 26
#define FRAMES 230
#define MAX_HEAT 36

static const int palette[] = {52, 88, 124, 160, 196, 202, 208, 214,
                              220, 226, 227, 228, 229, 230, 231};
static int heat[H][W];

int main(void) {
    srand(1993);
    for (int x = 0; x < W; x++) heat[H - 1][x] = MAX_HEAT;

    printf("\033[2J");
    for (int frame = 0; frame < FRAMES; frame++) {
        if (frame == FRAMES - 70)  // put the fire out
            for (int x = 0; x < W; x++) heat[H - 1][x] = 0;

        for (int y = 1; y < H; y++)
            for (int x = 0; x < W; x++) {
                int r = rand() % 10;
                int cool = r < 1 ? 0 : r < 5 ? 1 : 2;
                int dst = x - rand() % 3 + 1;  // wind
                if (dst < 0) dst = 0;
                if (dst >= W) dst = W - 1;
                int h = heat[y][x] - cool;
                heat[y - 1][dst] = h > 0 ? h : 0;
            }

        int last = -1;
        printf("\033[H");
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                int h = heat[y][x];
                if (h < 3) {
                    putchar(' ');
                    continue;
                }
                int c = palette[(h - 3) * 14 / (MAX_HEAT - 3)];
                if (c != last) {
                    printf("\033[38;5;%dm", c);
                    last = c;
                }
                fputs("█", stdout);
            }
            putchar('\n');
        }
        printf("\033[0m");
        fflush(stdout);
        usleep(33000);
    }
    printf("\033[1;31m  [+] fire extinguished\033[0m\n");
    return 0;
}
