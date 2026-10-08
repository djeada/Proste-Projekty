# A spinning torus drawn with ASCII characters: brighter character, more light.
from math import cos, sin

from term import COLS, ROWS, ink, show_text, start, text

RAMP = ".,-~:;=!*#$@"


def main():
    A = B = 0.0
    start(30)
    while True:
        zbuf = [[0.0] * COLS for _ in range(ROWS)]
        for row in text:
            row[:] = [" "] * COLS
        sA, cA, sB, cB = sin(A), cos(A), sin(B), cos(B)
        for j in range(90):
            sj, cj = sin(j * 0.07), cos(j * 0.07)
            h = cj + 2
            for i in range(314):
                si, ci = sin(i * 0.02), cos(i * 0.02)
                D = 1 / (si * h * sA + sj * cA + 5)
                t = si * h * cA - sj * sA
                x = COLS // 2 + int(32 * D * (ci * h * cB - t * sB))
                y = ROWS // 2 + int(14 * D * (ci * h * sB + t * cB))
                L = int(8 * ((sj * sA - si * cj * cA) * cB - si * cj * sA - sj * cA - ci * cj * sB))
                L = max(L, 0)
                if 0 <= x < COLS and 0 <= y < ROWS and D > zbuf[y][x]:
                    zbuf[y][x] = D
                    text[y][x] = RAMP[L]
                    ink[y][x] = L * 16 << 16 | (80 + L * 15) << 8 | L * 12
        show_text()
        A += 0.07
        B += 0.03


if __name__ == "__main__":
    main()
