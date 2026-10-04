# Demoscene plasma: four sine waves mixed into a 24-bit color field.
import sys
import time
from math import hypot, sin

W, H = 48, 26
FRAMES = 200


def main(frames=FRAMES):
    sys.stdout.write("\033[2J")
    for frame in range(frames):
        t = frame * 0.07
        out = ["\033[H"]
        for y in range(H):
            for x in range(W):
                px, py = x - W / 2, (y - H / 2) * 2.3  # square pixels
                v = (sin(px * 0.16 + t)
                     + sin(py * 0.13 - t * 1.3)
                     + sin((px + py) * 0.11 + t * 0.7)
                     + sin(hypot(px, py) * 0.18 - t * 1.6))
                hue = v * 0.8 + t * 0.5
                r = int(128 + 127 * sin(hue))
                g = int(128 + 127 * sin(hue + 2.094))
                b = int(128 + 127 * sin(hue + 4.189))
                out.append(f"\033[38;2;{r};{g};{b}m█")
            out.append("\n")
        out.append("\033[0m")
        sys.stdout.write("".join(out))
        sys.stdout.flush()
        time.sleep(0.033)
    print(f"\033[1;35m  [+] {frames} frames of plasma, zero textures\033[0m")


if __name__ == "__main__":
    main()
