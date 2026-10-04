// The Matrix "digital rain": one falling stream per column.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define W 48
#define H 26
#define FRAMES 190
#define TRAIL 14

static const int shade[TRAIL] = {231, 157, 120, 83, 46, 40, 40, 34, 34, 28, 28, 22, 22, 22};

static float new_speed(void) { return 0.3f + (rand() % 70) / 100.0f; }

int main(void) {
    char glyph[H][W];
    float head[W], speed[W];
    srand(1999);
    for (int x = 0; x < W; x++) {
        head[x] = -(rand() % (H * 2));
        speed[x] = new_speed();
        for (int y = 0; y < H; y++) glyph[y][x] = 33 + rand() % 94;
    }

    printf("\033[2J");
    for (int frame = 0; frame < FRAMES; frame++) {
        int last = -1;
        printf("\033[H");
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                int d = (int)head[x] - y;  // distance behind the stream's head
                if (d < 0 || d >= TRAIL) {
                    putchar(' ');
                    continue;
                }
                if (rand() % 15 == 0) glyph[y][x] = 33 + rand() % 94;
                if (shade[d] != last) {
                    printf("\033[38;5;%dm", shade[d]);
                    last = shade[d];
                }
                putchar(glyph[y][x]);
            }
            putchar('\n');
        }
        fflush(stdout);
        for (int x = 0; x < W; x++) {
            head[x] += speed[x];
            if (head[x] - TRAIL > H) {
                head[x] = -(rand() % H);
                speed[x] = new_speed();
            }
        }
        usleep(40000);
    }

    const char *lines[] = {"Wake up, Neo...", "The Matrix has you...",
                           "Follow the white rabbit."};
    printf("\033[2J\033[H\033[1;32m\n");
    for (int i = 0; i < 3; i++) {
        printf("  ");
        for (const char *c = lines[i]; *c; c++) {
            putchar(*c);
            fflush(stdout);
            usleep(70000);
        }
        printf("\n\n");
        usleep(500000);
    }
    printf("\033[0m");
    return 0;
}
