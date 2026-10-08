# Carves a random maze with depth-first search, then finds the shortest path with BFS.
from collections import deque

from term import H, W, pixels, rnd, seed, show_pixels, start

MW, MH = W - 1, H - 1
WALL, OPEN = -2, -1
RIPPLE = [0x20E0FF, 0x2098FF, 0x3060FF, 0x6040F0, 0x9030E0, 0x6040F0, 0x3060FF, 0x2098FF]


def color(maze, path, x, y, head):
    if (x, y) == (1, 1):
        return 0x40FF70
    if (x, y) == (MW - 2, MH - 2):
        return 0xFF4040
    if (x, y) == head:
        return 0xFF40FF
    if (x, y) in path:
        return 0xFFE040
    if maze[y][x] == WALL:
        return 0x384050
    if maze[y][x] >= 0:
        return RIPPLE[maze[y][x] // 4 % 8]
    return 0


def draw(maze, path, status, head=None):
    for y in range(H):
        for x in range(W):
            pixels[y][x] = color(maze, path, x, y, head) if x < MW and y < MH else 0
    show_pixels(status)


def carve(maze):
    dirs = [(0, -2), (2, 0), (0, 2), (-2, 0)]
    maze[1][1] = OPEN
    stack = [(1, 1)]
    while stack:
        x, y = stack[-1]
        for i in range(3, 0, -1):
            j = rnd(i + 1)
            dirs[i], dirs[j] = dirs[j], dirs[i]
        for dx, dy in dirs:
            nx, ny = x + dx, y + dy
            if 0 < nx < MW - 1 and 0 < ny < MH - 1 and maze[ny][nx] == WALL:
                maze[y + dy // 2][x + dx // 2] = OPEN
                maze[ny][nx] = OPEN
                stack.append((nx, ny))
                draw(maze, set(), " carving the maze (depth-first search)", (nx, ny))
                break
        else:
            stack.pop()


def solve(maze):
    goal = (MW - 2, MH - 2)
    came_from = {}
    layer = 0
    maze[1][1] = 0
    queue = deque([(1, 1)])
    while queue:
        x, y = queue.popleft()
        if maze[y][x] > layer:
            layer = maze[y][x]
            if layer % 2 == 0:
                draw(maze, set(), f" searching (breadth-first search): distance {layer}")
        if (x, y) == goal:
            break
        for dx, dy in ((0, -1), (1, 0), (0, 1), (-1, 0)):
            nx, ny = x + dx, y + dy
            if maze[ny][nx] == OPEN:
                maze[ny][nx] = maze[y][x] + 1
                came_from[nx, ny] = (x, y)
                queue.append((nx, ny))

    status = f" shortest path: {maze[goal[1]][goal[0]]} steps"
    path, cell = set(), goal
    while cell != (1, 1):
        path.add(cell)
        cell = came_from[cell]
        draw(maze, path, status)
    for _ in range(60):
        draw(maze, path, status)


def main():
    seed(2024)
    start(15)
    while True:
        maze = [[WALL] * MW for _ in range(MH)]
        carve(maze)
        solve(maze)


if __name__ == "__main__":
    main()
