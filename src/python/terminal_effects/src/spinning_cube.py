# A spinning cube rasterized with a z-buffer, one color per face.
import math
import sys
import time

W, H = 48, 26
FRAMES = 200
S = 10  # half the side of the cube


def cube_surface(step=0.5):
    steps = [-S + n * step for n in range(int(2 * S / step))]
    points = []
    for a in steps:
        for b in steps:
            points += [
                (a, b, -S, 196),   # front: red
                (S, b, a, 46),     # right: green
                (-S, b, -a, 33),   # left: blue
                (-a, b, S, 226),   # back: yellow
                (a, -S, -b, 201),  # bottom: magenta
                (a, S, b, 51),     # top: cyan
            ]
    return points


def main(frames=FRAMES):
    points = cube_surface()
    A = B = C = 0.0
    sys.stdout.write("\033[2J")
    for _ in range(frames):
        sA, cA, sB, cB = math.sin(A), math.cos(A), math.sin(B), math.cos(B)
        sC, cC = math.sin(C), math.cos(C)
        zbuf = [0.0] * (W * H)
        cell = [0] * (W * H)
        for i, j, k, color in points:
            # rotate the point around the x, y and z axes
            x = (j * sA * sB * cC - k * cA * sB * cC + j * cA * sC
                 + k * sA * sC + i * cB * cC)
            y = (j * cA * cC + k * sA * cC - j * sA * sB * sC
                 + k * cA * sB * sC - i * cB * sC)
            z = k * cA * cB - j * sA * cB + i * sB + 60
            ooz = 1 / z
            xp = int(W // 2 + 24 * ooz * x * 2.2)
            yp = int(H // 2 + 24 * ooz * y)
            idx = xp + yp * W
            if 0 <= xp < W and 0 <= yp < H and ooz > zbuf[idx]:
                zbuf[idx] = ooz
                cell[idx] = color

        last = -1
        out = ["\033[H"]
        for y in range(H):
            for c in cell[y * W:(y + 1) * W]:
                if not c:
                    out.append(" ")
                    continue
                if c != last:
                    out.append(f"\033[38;5;{c}m")
                    last = c
                out.append("█")
            out.append("\n")
        out.append("\033[0m")
        sys.stdout.write("".join(out))
        sys.stdout.flush()
        A, B, C = A + 0.05, B + 0.05, C + 0.01
        time.sleep(0.03)


if __name__ == "__main__":
    main()
