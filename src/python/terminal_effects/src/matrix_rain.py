# The Matrix "digital rain": one falling stream per column.
import random
import sys
import time

W, H = 48, 26
FRAMES = 190
TRAIL = 14
SHADE = [231, 157, 120, 83, 46, 40, 40, 34, 34, 28, 28, 22, 22, 22]
LINES = ["Wake up, Neo...", "The Matrix has you...", "Follow the white rabbit."]


def new_speed():
    return 0.3 + random.randrange(70) / 100


def new_glyph():
    return chr(random.randrange(33, 127))


def main(frames=FRAMES):
    random.seed(1999)
    head = [-random.randrange(H * 2) for _ in range(W)]
    speed = [new_speed() for _ in range(W)]
    glyph = [[new_glyph() for _ in range(W)] for _ in range(H)]

    sys.stdout.write("\033[2J")
    for _ in range(frames):
        last = -1
        out = ["\033[H"]
        for y in range(H):
            for x in range(W):
                d = int(head[x]) - y  # distance behind the stream's head
                if not 0 <= d < TRAIL:
                    out.append(" ")
                    continue
                if random.randrange(15) == 0:
                    glyph[y][x] = new_glyph()
                if SHADE[d] != last:
                    out.append(f"\033[38;5;{SHADE[d]}m")
                    last = SHADE[d]
                out.append(glyph[y][x])
            out.append("\n")
        sys.stdout.write("".join(out))
        sys.stdout.flush()
        for x in range(W):
            head[x] += speed[x]
            if head[x] - TRAIL > H:
                head[x] = -random.randrange(H)
                speed[x] = new_speed()
        time.sleep(0.04)

    print("\033[2J\033[H\033[1;32m")
    for line in LINES:
        print("  ", end="")
        for ch in line:
            print(ch, end="", flush=True)
            time.sleep(0.07)
        print("\n")
        time.sleep(0.5)
    print("\033[0m", end="")


if __name__ == "__main__":
    main()
