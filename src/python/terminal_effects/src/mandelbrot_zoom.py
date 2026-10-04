# Deep zoom into the Mandelbrot set's "seahorse valley".
import sys
import time

W, H = 48, 26
FRAMES = 160
PALETTE = [21, 27, 33, 39, 45, 51, 87, 123, 159, 195, 231, 229, 227,
           226, 220, 214, 208, 202, 196, 199, 201, 165, 129, 93, 57]
CENTER = complex(-0.743643887037151, 0.131825904205330)


def main(frames=FRAMES):
    scale = 3.2
    sys.stdout.write("\033[2J")
    for frame in range(frames):
        max_iter, last = 100 + frame * 6, -1
        out = ["\033[H"]
        for y in range(H):
            for x in range(W):
                re = (x - W / 2) * scale / W
                im = (y - H / 2) * scale * 1.25 / H
                c = CENTER + complex(re, im)
                z, it = 0j, 0
                while abs(z) < 2 and it < max_iter:
                    z = z * z + c
                    it += 1
                if it == max_iter:
                    out.append(" ")
                    continue
                color = PALETTE[(it + frame // 2) % len(PALETTE)]
                if color != last:
                    out.append(f"\033[38;5;{color}m")
                    last = color
                out.append("█")
            out.append("\n")
        out.append(f"\033[0m  zoom {3.2 / scale:.0f}x  |  {max_iter} iterations\n")
        sys.stdout.write("".join(out))
        sys.stdout.flush()
        scale *= 0.94
        time.sleep(0.03)


if __name__ == "__main__":
    main()
