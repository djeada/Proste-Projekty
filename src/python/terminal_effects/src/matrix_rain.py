# The "digital rain" from The Matrix: one falling stream of characters per column.
from term import COLS, ROWS, ink, rnd, seed, show_text, start, text

SHADE = [
    0xEAFFEA, 0x9CFF9C, 0x4CF04C, 0x22D022, 0x18B018, 0x149414,
    0x107A10, 0x0C640C, 0x0A520A, 0x084208, 0x063406, 0x042804,
]
TRAIL = len(SHADE)


def main():
    seed(1999)
    head, speed = [], []
    glyph = [[" "] * COLS for _ in range(ROWS)]
    for x in range(COLS):
        head.append(-rnd(ROWS * 4))
        speed.append(1 + rnd(3))
        for y in range(ROWS):
            glyph[y][x] = chr(33 + rnd(94))
    start(50)
    frame = 0
    while True:
        for row in text:
            row[:] = [" "] * COLS
        for x in range(COLS):
            if frame % speed[x] == 0:
                head[x] += 1
            if head[x] - TRAIL > ROWS:
                head[x] = -rnd(ROWS)
                speed[x] = 1 + rnd(3)
            for d in range(TRAIL):
                y = head[x] - d
                if not 0 <= y < ROWS:
                    continue
                if rnd(20) == 0:
                    glyph[y][x] = chr(33 + rnd(94))
                text[y][x] = glyph[y][x]
                ink[y][x] = SHADE[d]
        show_text()
        frame += 1


if __name__ == "__main__":
    main()
