# 3D starfield accelerating to warp speed, with motion streaks.
import random
import sys
import time

W, H = 48, 26
STARS = 260
FRAMES = 210
GLYPHS = "@*+."  # from near to far
COLORS = [231, 195, 153, 244]


def spawn(z):
    return random.uniform(-1, 1), random.uniform(-1, 1), z


def put(cells, x, y, z, ch, color):
    px = int(W // 2 + x / z * W * 0.35)
    py = int(H // 2 + y / z * W * 0.35 / 2.3)
    if 0 <= px < W and 0 <= py < H and z <= cells[py][px][0]:
        cells[py][px] = (z, ch, color)


def main(frames=FRAMES):
    random.seed(42)
    stars = [spawn(random.uniform(0.05, 1.05)) for _ in range(STARS)]

    sys.stdout.write("\033[2J")
    for frame in range(frames):
        p = frame / frames
        speed = 0.004 + 0.035 * p * p
        cells = [[(1e9, " ", 0)] * W for _ in range(H)]  # (depth, glyph, color)

        for i, (x, y, z) in enumerate(stars):
            z -= speed
            if z < 0.02:
                x, y, z = spawn(1.05)
            stars[i] = (x, y, z)
            trail = int(speed * 400)  # streaks grow with speed
            for k in range(trail, 0, -1):
                put(cells, x, y, z + k * speed * 0.6, ".", 240 + k % 4)
            band = min(int(z * 4), 3)
            put(cells, x, y, z, GLYPHS[band], COLORS[band])

        last = -1
        out = ["\033[H"]
        for row in cells:
            for _, ch, color in row:
                if ch != " " and color != last:
                    out.append(f"\033[38;5;{color}m")
                    last = color
                out.append(ch)
            out.append("\n")
        out.append(f"\033[0m  warp factor {1 + 8.9 * p * p:.1f}\n")
        sys.stdout.write("".join(out))
        sys.stdout.flush()
        time.sleep(0.03)
    print("\033[2J\033[H\n\033[1;36m  [+] arrived at Alpha Centauri\033[0m")


if __name__ == "__main__":
    main()
