# Carve a random maze with depth-first search, then solve it with BFS.
import random
import sys
import time
from collections import deque

MW, MH = 47, 25
WALL, OPEN = -2, -1
RIPPLE = [51, 45, 39, 33, 27, 21, 57, 93, 129, 165, 201,
          165, 129, 93, 57, 21, 27, 33, 39, 45]


def draw(maze, path=(), head=None):
    last = -1
    out = ["\033[H"]
    for y, row in enumerate(maze):
        for x, v in enumerate(row):
            cell = (x, y)
            c = (46 if cell == (1, 1) else
                 196 if cell == (MW - 2, MH - 2) else
                 226 if cell in path else
                 201 if cell == head else
                 238 if v == WALL else
                 RIPPLE[v // 6 % 20] if v >= 0 else 0)
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


def carve(maze):
    maze[1][1] = OPEN
    stack = [(1, 1)]
    dirs = [(0, -2), (2, 0), (0, 2), (-2, 0)]
    while stack:
        x, y = stack[-1]
        random.shuffle(dirs)
        for dx, dy in dirs:
            nx, ny = x + dx, y + dy
            if 0 < nx < MW - 1 and 0 < ny < MH - 1 and maze[ny][nx] == WALL:
                maze[y + dy // 2][x + dx // 2] = OPEN
                maze[ny][nx] = OPEN
                stack.append((nx, ny))
                draw(maze, head=(nx, ny))
                time.sleep(0.009)
                break
        else:  # dead end: backtrack
            stack.pop()


def solve(maze):
    goal = (MW - 2, MH - 2)
    parent = {}
    layer = 0
    maze[1][1] = 0
    queue = deque([(1, 1)])
    while queue:
        x, y = queue.popleft()
        if maze[y][x] > layer:
            layer = maze[y][x]
            if layer % 2 == 0:
                draw(maze)
                time.sleep(0.02)
        if (x, y) == goal:
            break
        for dx, dy in ((0, -1), (1, 0), (0, 1), (-1, 0)):
            nx, ny = x + dx, y + dy
            if maze[ny][nx] == OPEN:
                maze[ny][nx] = maze[y][x] + 1
                parent[nx, ny] = (x, y)
                queue.append((nx, ny))

    path, cell = set(), goal
    while cell != (1, 1):  # walk back
        path.add(cell)
        cell = parent[cell]
        draw(maze, path)
        time.sleep(0.012)
    print(f"\033[1;33m  [+] shortest path: {maze[MH - 2][MW - 2]} steps\033[0m")


def main():
    random.seed(2024)
    maze = [[WALL] * MW for _ in range(MH)]
    sys.stdout.write("\033[2J")
    carve(maze)
    time.sleep(0.4)
    solve(maze)


if __name__ == "__main__":
    main()
