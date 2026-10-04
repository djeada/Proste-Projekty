// Demoscene plasma: four sine waves mixed into a 24-bit color field.
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define W 48
#define H 26
#define FRAMES 200

int main(void) {
    printf("\033[2J");
    for (int frame = 0; frame < FRAMES; frame++) {
        float t = frame * 0.07f;
        printf("\033[H");
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                float px = x - W / 2.0f, py = (y - H / 2.0f) * 2.3f;  // square pixels
                float v = sinf(px * 0.16f + t)
                        + sinf(py * 0.13f - t * 1.3f)
                        + sinf((px + py) * 0.11f + t * 0.7f)
                        + sinf(sqrtf(px * px + py * py) * 0.18f - t * 1.6f);
                float hue = v * 0.8f + t * 0.5f;
                int r = (int)(128 + 127 * sinf(hue));
                int g = (int)(128 + 127 * sinf(hue + 2.094f));
                int b = (int)(128 + 127 * sinf(hue + 4.189f));
                printf("\033[38;2;%d;%d;%dm█", r, g, b);
            }
            putchar('\n');
        }
        printf("\033[0m");
        fflush(stdout);
        usleep(33000);
    }
    printf("\033[1;35m  [+] %d frames of plasma, zero textures\033[0m\n", FRAMES);
    return 0;
}
