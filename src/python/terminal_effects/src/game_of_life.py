# Conway's Game of Life on a wrapping grid, cells colored by age.
import random
import sys
import time

W, H = 24, 25
GENERATIONS = 230
AGE_COLOR = [231, 195, 159, 123, 87, 51, 45, 39, 33, 27, 21, 20]
GLIDER = [(0, 1), (1, 2), (2, 0), (2, 1), (2, 2)]


def drop_glider(grid):
    gx, gy = random.randrange(W), random.randrange(H)
    for i, j in GLIDER:
        grid[(gy + i) % H][(gx + j) % W] = 1


def main(generations=GENERATIONS):
    random.seed(42)
    # value = age, 0 is dead
    grid = [[int(random.random() < 0.33) for _ in range(W)] for _ in range(H)]

    sys.stdout.write("\033[2J")
    for gen in range(generations):
        alive, last = 0, -1
        out = ["\033[H"]
        for row in grid:
            for age in row:
                if not age:
                    out.append("  ")
                    continue
                alive += 1
                c = AGE_COLOR[min(age, 12) - 1]
                if c != last:
                    out.append(f"\033[38;5;{c}m")
                    last = c
                out.append("██")
            out.append("\n")
        out.append(f"\033[0m  generation {gen:3d}  |  alive {alive:3d}\n")
        sys.stdout.write("".join(out))
        sys.stdout.flush()

        nxt = [[0] * W for _ in range(H)]
        for y in range(H):
            for x in range(W):
                n = sum(grid[(y + dy) % H][(x + dx) % W] > 0
                        for dy in (-1, 0, 1) for dx in (-1, 0, 1) if dy or dx)
                age = grid[y][x]
                nxt[y][x] = age + 1 if n == 3 or (age and n == 2) else 0
        grid = nxt
        if gen % 25 == 24:  # keep the world lively
            drop_glider(grid)
        time.sleep(0.035)


if __name__ == "__main__":
    main()
