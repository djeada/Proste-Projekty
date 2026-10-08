// Demoscene plasma: four sine waves added together and turned into colors.
#include <math.h>

#include "term.h"

int main(int argc, char **argv) {
    start(argc, argv, 30);
    for (int frame = 0;; frame++) {
        double t = frame * 0.07;
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++) {
                double px = x - W / 2.0, py = y - H / 2.0;
                double v = sin(px * 0.12 + t) + sin(py * 0.1 - t * 1.3) + sin((px + py) * 0.08 + t * 0.7) +
                           sin(sqrt(px * px + py * py) * 0.15 - t * 1.6);
                double hue = v * 0.8 + t * 0.5;
                int r = (int)(128 + 127 * sin(hue));
                int g = (int)(128 + 127 * sin(hue + 2.094));
                int b = (int)(128 + 127 * sin(hue + 4.189));
                pixels[y][x] = (uint32_t)(r << 16 | g << 8 | b);
            }
        show_pixels("");
    }
}
