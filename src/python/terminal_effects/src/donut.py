# Spinning 3D torus in pure Python: no libraries, just math.
import math
import sys
import time

W, H = 50, 24
FRAMES = 170
RAMP = ".,-~:;=!*#$@"
GLOW = [22, 28, 34, 40, 46, 82, 118, 120, 157, 194, 231, 231]
PIXELS = [f"\033[38;5;{c}m{ch}" for c, ch in zip(GLOW, RAMP)]
# sin and cos around the tube (j) and around the ring (i), computed once
TUBE = [(math.sin(j * 0.07), math.cos(j * 0.07)) for j in range(90)]
RING = [(math.sin(i * 0.02), math.cos(i * 0.02)) for i in range(314)]


def main(frames=FRAMES):
    A = B = 0.0
    print("\033[1;32m[*] spawning torus...\033[0m", flush=True)
    time.sleep(0.6)
    sys.stdout.write("\033[2J")

    for _ in range(frames):
        screen = [" "] * (W * H)
        zbuf = [0.0] * (W * H)
        sA, cA, sB, cB = math.sin(A), math.cos(A), math.sin(B), math.cos(B)
        for sj, cj in TUBE:
            h = cj + 2
            for si, ci in RING:
                D = 1 / (si * h * sA + sj * cA + 5)
                t = si * h * cA - sj * sA
                x = W // 2 + int(30 * D * (ci * h * cB - t * sB))
                y = H // 2 + int(13 * D * (ci * h * sB + t * cB))
                N = int(8 * ((sj * sA - si * cj * cA) * cB
                             - si * cj * sA - sj * cA - ci * cj * sB))
                o = x + W * y
                if 0 <= y < H and 0 <= x < W - 1 and D > zbuf[o]:
                    zbuf[o] = D
                    screen[o] = PIXELS[max(N, 0)]
        rows = ("".join(screen[y * W:(y + 1) * W - 1]) for y in range(H))
        sys.stdout.write("\033[H" + "\n".join(rows) + "\n\033[0m")
        sys.stdout.flush()
        A += 0.07
        B += 0.03
        time.sleep(0.033)
    print(f"\033[1;32m[+] {frames} frames rendered. no GPU needed.\033[0m")


if __name__ == "__main__":
    main()
