# Langton's ant: on an empty cell turn right, on a filled cell turn left,
# flip the cell and step forward. After ~10,000 steps of chaos it builds a "highway".
from term import H, W, pixels, show_pixels, start

DX, DY = (0, 1, 0, -1), (-1, 0, 1, 0)


def main():
    start(30)
    while True:
        x, y, direction = W // 2, H // 2, 0
        cell = [[0] * W for _ in range(H)]
        for step in range(12000):
            direction = (direction + (3 if cell[y][x] else 1)) % 4
            cell[y][x] = 1 - cell[y][x]
            x = (x + DX[direction]) % W
            y = (y + DY[direction]) % H
            if step % 60 == 0:
                for py in range(H):
                    for px in range(W):
                        pixels[py][px] = 0x30C0A0 if cell[py][px] else 0
                pixels[y][x] = 0xFF3050
                show_pixels(f" step {step}")


if __name__ == "__main__":
    main()
