# The PSX Doom fire: heat rises, drifts sideways and cools at random.
import random
import sys
import time

W, H = 48, 26
FRAMES = 230
MAX_HEAT = 36
PALETTE = [52, 88, 124, 160, 196, 202, 208, 214, 220, 226, 227, 228, 229, 230, 231]


def main(frames=FRAMES):
    random.seed(1993)
    heat = [[0] * W for _ in range(H)]
    heat[H - 1] = [MAX_HEAT] * W

    sys.stdout.write("\033[2J")
    for frame in range(frames):
        if frame == frames - 70:  # put the fire out
            heat[H - 1] = [0] * W

        for y in range(1, H):
            for x in range(W):
                r = random.randrange(10)
                cool = 0 if r < 1 else 1 if r < 5 else 2
                dst = min(max(x + random.randint(-1, 1), 0), W - 1)  # wind
                heat[y - 1][dst] = max(heat[y][x] - cool, 0)

        last = -1
        out = ["\033[H"]
        for row in heat:
            for h in row:
                if h < 3:
                    out.append(" ")
                    continue
                c = PALETTE[(h - 3) * 14 // (MAX_HEAT - 3)]
                if c != last:
                    out.append(f"\033[38;5;{c}m")
                    last = c
                out.append("█")
            out.append("\n")
        out.append("\033[0m")
        sys.stdout.write("".join(out))
        sys.stdout.flush()
        time.sleep(0.033)
    print("\033[1;31m  [+] fire extinguished\033[0m")


if __name__ == "__main__":
    main()
