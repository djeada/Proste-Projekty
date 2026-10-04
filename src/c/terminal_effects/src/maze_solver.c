// Carve a random maze with depth-first search, then solve it with BFS.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MW 47
#define MH 25
#define WALL (-2)
#define OPEN (-1)

static int maze[MH][MW], on_path[MH][MW];
static int px[MH][MW], py[MH][MW];
static const int ripple[] = {51, 45, 39, 33, 27, 21, 57, 93, 129, 165, 201, 165, 129, 93, 57, 21, 27, 33, 39, 45};

static void draw(int hx, int hy) {
    int last = -1;
    printf("\033[H");
    for (int y = 0; y < MH; y++) {
        for (int x = 0; x < MW; x++) {
            int v = maze[y][x], c;
            if (x == 1 && y == 1) c = 46;
            else if (x == MW - 2 && y == MH - 2) c = 196;
            else if (on_path[y][x]) c = 226;
            else if (x == hx && y == hy) c = 201;
            else if (v == WALL) c = 238;
            else if (v >= 0) c = ripple[(v / 6) % 20];
            else c = 0;
            if (!c) {
                putchar(' ');
                continue;
            }
            if (c != last) {
                printf("\033[38;5;%dm", c);
                last = c;
            }
            fputs("█", stdout);
        }
        putchar('\n');
    }
    printf("\033[0m");
    fflush(stdout);
}

static void carve(void) {
    static int stack[MW * MH][2];
    int top = 0, dirs[4][2] = {{0, -2}, {2, 0}, {0, 2}, {-2, 0}};
    maze[1][1] = OPEN;
    stack[top][0] = 1, stack[top][1] = 1, top++;
    while (top) {
        int x = stack[top - 1][0], y = stack[top - 1][1];
        for (int i = 3; i > 0; i--) {  // shuffle directions
            int j = rand() % (i + 1), tx = dirs[i][0], ty = dirs[i][1];
            dirs[i][0] = dirs[j][0], dirs[i][1] = dirs[j][1];
            dirs[j][0] = tx, dirs[j][1] = ty;
        }
        int moved = 0;
        for (int i = 0; i < 4 && !moved; i++) {
            int nx = x + dirs[i][0], ny = y + dirs[i][1];
            if (nx > 0 && nx < MW - 1 && ny > 0 && ny < MH - 1 && maze[ny][nx] == WALL) {
                maze[y + dirs[i][1] / 2][x + dirs[i][0] / 2] = OPEN;
                maze[ny][nx] = OPEN;
                stack[top][0] = nx, stack[top][1] = ny, top++;
                moved = 1;
                draw(nx, ny);
                usleep(9000);
            }
        }
        if (!moved) top--;
    }
}

static void solve(void) {
    static int queue[MW * MH][2];
    int head = 0, tail = 0, layer = 0;
    maze[1][1] = 0;
    queue[tail][0] = 1, queue[tail][1] = 1, tail++;
    while (head < tail) {
        int x = queue[head][0], y = queue[head][1];
        head++;
        if (maze[y][x] > layer) {
            layer = maze[y][x];
            if (layer % 2 == 0) {
                draw(-1, -1);
                usleep(20000);
            }
        }
        if (x == MW - 2 && y == MH - 2) break;
        int d[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        for (int i = 0; i < 4; i++) {
            int nx = x + d[i][0], ny = y + d[i][1];
            if (maze[ny][nx] == OPEN) {
                maze[ny][nx] = maze[y][x] + 1;
                px[ny][nx] = x, py[ny][nx] = y;
                queue[tail][0] = nx, queue[tail][1] = ny, tail++;
            }
        }
    }
    for (int x = MW - 2, y = MH - 2; !(x == 1 && y == 1);) {  // walk back
        on_path[y][x] = 1;
        int nx = px[y][x], ny = py[y][x];
        x = nx, y = ny;
        draw(-1, -1);
        usleep(12000);
    }
    printf("\033[1;33m  [+] shortest path: %d steps\033[0m\n", maze[MH - 2][MW - 2]);
}

int main(void) {
    srand(2024);
    for (int y = 0; y < MH; y++)
        for (int x = 0; x < MW; x++) maze[y][x] = WALL;
    printf("\033[2J");
    carve();
    usleep(400000);
    solve();
    return 0;
}
