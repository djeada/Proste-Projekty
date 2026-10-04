// 3D starfield accelerating to warp speed, with motion streaks.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define W 48
#define H 26
#define STARS 260
#define FRAMES 210

static float sx[STARS], sy[STARS], sz[STARS];
static char screen[H][W];
static int color[H][W];
static float depth[H][W];

static void spawn(int i, float z) {
    sx[i] = (rand() % 2000 - 1000) / 1000.0f;
    sy[i] = (rand() % 2000 - 1000) / 1000.0f;
    sz[i] = z;
}

static void put(float x, float y, float z, char ch, int c) {
    int px = (int)(W / 2 + x / z * W * 0.35f);
    int py = (int)(H / 2 + y / z * W * 0.35f / 2.3f);
    if (px < 0 || px >= W || py < 0 || py >= H || z > depth[py][px]) return;
    depth[py][px] = z;
    screen[py][px] = ch;
    color[py][px] = c;
}

int main(void) {
    srand(42);
    for (int i = 0; i < STARS; i++) spawn(i, 0.05f + (rand() % 1000) / 1000.0f);

    printf("\033[2J");
    for (int frame = 0; frame < FRAMES; frame++) {
        float p = (float)frame / FRAMES;
        float speed = 0.004f + 0.035f * p * p;
        memset(screen, ' ', sizeof screen);
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) depth[y][x] = 1e9f;

        for (int i = 0; i < STARS; i++) {
            sz[i] -= speed;
            if (sz[i] < 0.02f) spawn(i, 1.05f);
            int trail = (int)(speed * 400);  // streaks grow with speed
            for (int k = trail; k >= 1; k--)
                put(sx[i], sy[i], sz[i] + k * speed * 0.6f, '.', 240 + k % 4);
            float z = sz[i];
            put(sx[i], sy[i], z, z < 0.25f ? '@' : z < 0.5f ? '*' : z < 0.75f ? '+' : '.',
                z < 0.25f ? 231 : z < 0.5f ? 195 : z < 0.75f ? 153 : 244);
        }

        int last = -1;
        printf("\033[H");
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                if (screen[y][x] != ' ' && color[y][x] != last) {
                    printf("\033[38;5;%dm", color[y][x]);
                    last = color[y][x];
                }
                putchar(screen[y][x]);
            }
            putchar('\n');
        }
        printf("\033[0m  warp factor %.1f\n", 1 + 8.9f * p * p);
        fflush(stdout);
        usleep(30000);
    }
    printf("\033[2J\033[H\n\033[1;36m  [+] arrived at Alpha Centauri\033[0m\n");
    return 0;
}
