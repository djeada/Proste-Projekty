# Conway's Game of Life on a wrapping board, cells colored by age.
from term import H, W, pixels, rnd, seed, show_pixels, start

AGE_COLOR = [0xFFFFFF, 0xC8FFF4, 0x8EF5E6, 0x4FD8E0, 0x2AA8D8, 0x2A72C8, 0x3044A8, 0x2A2A80]
GLIDER = [(0, 1), (1, 2), (2, 0), (2, 1), (2, 2)]


def drop_glider(grid):
    gx, gy = rnd(W), rnd(H)
    for dy, dx in GLIDER:
        grid[(gy + dy) % H][(gx + dx) % W] = 1


def main():
    seed(42)
    grid = [[int(rnd(3) == 0) for _ in range(W)] for _ in range(H)]
    start(40)
    gen = 0
    while True:
        alive = 0
        for y in range(H):
            for x in range(W):
                age = grid[y][x]
                alive += age > 0
                pixels[y][x] = AGE_COLOR[min(age, 8) - 1] if age else 0
        show_pixels(f" generation {gen}, alive {alive}")

        nxt = [[0] * W for _ in range(H)]
        for y in range(H):
            for x in range(W):
                n = 0
                for dy in (-1, 0, 1):
                    for dx in (-1, 0, 1):
                        if (dy or dx) and grid[(y + dy) % H][(x + dx) % W]:
                            n += 1
                age = grid[y][x]
                nxt[y][x] = age + 1 if n == 3 or (age and n == 2) else 0
        grid = nxt
        if gen % 20 == 19:
            drop_glider(grid)
        gen += 1


if __name__ == "__main__":
    main()
