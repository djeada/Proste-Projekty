# Demoscene plasma: four sine waves added together and turned into colors.
from math import sin, sqrt

from term import H, W, pixels, show_pixels, start


def main():
    start(30)
    frame = 0
    while True:
        t = frame * 0.07
        for y in range(H):
            for x in range(W):
                px, py = x - W / 2, y - H / 2
                v = (sin(px * 0.12 + t) + sin(py * 0.1 - t * 1.3) + sin((px + py) * 0.08 + t * 0.7)
                     + sin(sqrt(px * px + py * py) * 0.15 - t * 1.6))
                hue = v * 0.8 + t * 0.5
                r = int(128 + 127 * sin(hue))
                g = int(128 + 127 * sin(hue + 2.094))
                b = int(128 + 127 * sin(hue + 4.189))
                pixels[y][x] = r << 16 | g << 8 | b
        show_pixels()
        frame += 1


if __name__ == "__main__":
    main()
