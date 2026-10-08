# Flying through a 3D starfield: the faster we go, the longer the streaks.
from term import H, W, pixels, rnd, seed, show_pixels, start

STARS = 300


def spawn(z):
    x = (rnd(2001) - 1000) / 1000
    y = (rnd(2001) - 1000) / 1000
    return [x, y, z]


def put(bright, x, y, z, b):
    px, py = W // 2 + int(x / z * 26), H // 2 + int(y / z * 26)
    if 0 <= px < W and 0 <= py < H and b > bright[py][px]:
        bright[py][px] = b


def main():
    seed(42)
    stars = [spawn(0.05 + rnd(1000) / 1000) for _ in range(STARS)]
    start(30)
    frame = 0
    while True:
        p = frame / 150 if frame < 150 else (300 - frame) / 150
        speed = 0.003 + 0.03 * p * p
        trail = int(speed * 300)
        bright = [[0] * W for _ in range(H)]
        for i, star in enumerate(stars):
            star[2] -= speed
            if star[2] < 0.02:
                star = stars[i] = spawn(1.05)
            x, y, z = star
            b = int((1.1 - z) * 230)
            for k in range(trail, 0, -1):
                put(bright, x, y, z + k * speed * 0.6, b // (k + 1))
            put(bright, x, y, z, b)
        for y in range(H):
            for x in range(W):
                b = bright[y][x]
                pixels[y][x] = (b * 3 // 4) << 16 | (b * 7 // 8) << 8 | b
        show_pixels(f" warp factor {1 + int(p * 8)}")
        frame = (frame + 1) % 300


if __name__ == "__main__":
    main()
