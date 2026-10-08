# Zooms into the Mandelbrot set near the "seahorse valley".
from term import H, W, pixels, show_pixels, start

PALETTE = [
    0x000764, 0x02308C, 0x0A5BB4, 0x2085D2, 0x4CB0E6, 0x8BD6F2, 0xD2F0F7, 0xFFF7C8,
    0xFFE07A, 0xFFB52E, 0xF4800C, 0xD94F07, 0xA82808, 0x7A1240, 0x4A0A6E, 0x1E0368,
]


def main():
    cx, cy = -0.743643887037151, 0.131825904205330
    scale = 3.0
    frame = 0
    start(30)
    while True:
        max_iter = 60 + frame * 3
        for y in range(H):
            for x in range(W):
                re = cx + (x - W / 2) * scale / W
                im = cy + (y - H / 2) * scale / W
                zr = zi = 0.0
                it = 0
                while zr * zr + zi * zi < 4 and it < max_iter:
                    zr, zi = zr * zr - zi * zi + re, 2 * zr * zi + im
                    it += 1
                pixels[y][x] = 0 if it == max_iter else PALETTE[(it + frame) % 16]
        show_pixels(f" zoom {int(3 / scale)}x, {max_iter} iterations")
        scale *= 0.93
        frame += 1
        if frame == 130:
            frame = 0
            scale = 3.0


if __name__ == "__main__":
    main()
