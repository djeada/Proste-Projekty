// The "digital rain" from The Matrix: one falling stream of characters per column.
#include <string.h>

#include "term.h"

#define TRAIL 12

static const uint32_t shade[TRAIL] = {
    0xeaffea, 0x9cff9c, 0x4cf04c, 0x22d022, 0x18b018, 0x149414,
    0x107a10, 0x0c640c, 0x0a520a, 0x084208, 0x063406, 0x042804,
};

int main(int argc, char **argv) {
    char glyph[ROWS][COLS];
    int head[COLS], speed[COLS];
    seed(1999);
    for (int x = 0; x < COLS; x++) {
        head[x] = -rnd(ROWS * 4);
        speed[x] = 1 + rnd(3);
        for (int y = 0; y < ROWS; y++) glyph[y][x] = (char)(33 + rnd(94));
    }
    start(argc, argv, 50);
    for (int frame = 0;; frame++) {
        memset(text, ' ', sizeof text);
        for (int x = 0; x < COLS; x++) {
            if (frame % speed[x] == 0) head[x]++;
            if (head[x] - TRAIL > ROWS) {
                head[x] = -rnd(ROWS);
                speed[x] = 1 + rnd(3);
            }
            for (int d = 0; d < TRAIL; d++) {
                int y = head[x] - d;
                if (y < 0 || y >= ROWS) continue;
                if (rnd(20) == 0) glyph[y][x] = (char)(33 + rnd(94));
                text[y][x] = glyph[y][x];
                ink[y][x] = shade[d];
            }
        }
        show_text("");
    }
}
