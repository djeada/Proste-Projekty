// Deep zoom into the Mandelbrot set's "seahorse valley".
#include <stdio.h>
#include <unistd.h>

#define W 48
#define H 26
#define FRAMES 160

static const int palette[] = {
    21, 27, 33, 39, 45, 51, 87, 123, 159, 195, 231, 229, 227,
    226, 220, 214, 208, 202, 196, 199, 201, 165, 129, 93, 57,
};
#define NCOLORS (int)(sizeof palette / sizeof palette[0])

int main(void) {
    const double cx = -0.743643887037151, cy = 0.131825904205330;
    double scale = 3.2;

    printf("\033[2J");
    for (int frame = 0; frame < FRAMES; frame++) {
        int max_iter = 100 + frame * 6, last = -1;
        printf("\033[H");
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                double re = cx + (x - W / 2.0) * scale / W;
                double im = cy + (y - H / 2.0) * scale * 1.25 / H;
                double zr = 0, zi = 0;
                int it = 0;
                while (zr * zr + zi * zi < 4 && it < max_iter) {
                    double t = zr * zr - zi * zi + re;
                    zi = 2 * zr * zi + im;
                    zr = t;
                    it++;
                }
                if (it == max_iter) {
                    putchar(' ');
                    continue;
                }
                int color = palette[(it + frame / 2) % NCOLORS];
                if (color != last) {
                    printf("\033[38;5;%dm", color);
                    last = color;
                }
                fputs("█", stdout);
            }
            putchar('\n');
        }
        printf("\033[0m  zoom %.0fx  |  %d iterations\n", 3.2 / scale, max_iter);
        fflush(stdout);
        scale *= 0.94;
        usleep(30000);
    }
    return 0;
}
