/* Rules of 2048: sliding, merging, spawning and win/lose checks. */
#include "game_2048.h"

#include <string.h>

#define N G2048_SIZE

int slide_row(int line[N]) {
    int tiles[N] = {0};
    int count = 0;
    for (int k = 0; k < N; k++) {
        if (line[k] != 0) tiles[count++] = line[k];
    }

    int out[N] = {0};
    int points = 0;
    int n = 0;
    for (int i = 0; i < count; i++) {
        if (i + 1 < count && tiles[i] == tiles[i + 1]) {
            /* Two equal tiles merge once; the next tile is not looked at again. */
            out[n++] = 2 * tiles[i];
            points += 2 * tiles[i];
            i++;
        } else {
            out[n++] = tiles[i];
        }
    }
    memcpy(line, out, sizeof out);
    return points;
}

/* The k-th cell of line `line`, counting from the side the tiles slide towards. */
static void line_cell(Direction dir, int line, int k, int *row, int *col) {
    if (dir == DIR_LEFT) {
        *row = line;
        *col = k;
    } else if (dir == DIR_RIGHT) {
        *row = line;
        *col = N - 1 - k;
    } else if (dir == DIR_UP) {
        *row = k;
        *col = line;
    } else {
        *row = N - 1 - k;
        *col = line;
    }
}

int board_slide(int cells[N][N], Direction dir) {
    int points = 0;
    for (int line = 0; line < N; line++) {
        int values[N];
        for (int k = 0; k < N; k++) {
            int row, col;
            line_cell(dir, line, k, &row, &col);
            values[k] = cells[row][col];
        }
        points += slide_row(values);
        for (int k = 0; k < N; k++) {
            int row, col;
            line_cell(dir, line, k, &row, &col);
            cells[row][col] = values[k];
        }
    }
    return points;
}

int game_spawn_tile(Game *g, RandomBelow random_below) {
    int empty[N * N];
    int count = 0;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (g->cells[r][c] == 0) empty[count++] = r * N + c;
        }
    }
    if (count == 0) return 0;

    int pick = empty[random_below(count)];
    g->cells[pick / N][pick % N] = (random_below(10) == 0) ? 4 : 2;
    return 1;
}

void game_start(Game *g, RandomBelow random_below) {
    memset(g, 0, sizeof *g);
    game_spawn_tile(g, random_below);
    game_spawn_tile(g, random_below);
}

int game_can_move(const Game *g) {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            int v = g->cells[r][c];
            if (v == 0) return 1;
            if (r + 1 < N && g->cells[r + 1][c] == v) return 1;
            if (c + 1 < N && g->cells[r][c + 1] == v) return 1;
        }
    }
    return 0;
}

void game_keep_playing(Game *g) {
    g->keep_playing = 1;
}

Status game_status(const Game *g) {
    int has_2048 = 0;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (g->cells[r][c] >= 2048) has_2048 = 1;
        }
    }
    if (has_2048 && !g->keep_playing) return STATUS_WON;
    if (!game_can_move(g)) return STATUS_LOST;
    return STATUS_PLAYING;
}

int game_move(Game *g, Direction dir, RandomBelow random_below) {
    if (game_status(g) != STATUS_PLAYING) return 0;

    int before[N][N];
    memcpy(before, g->cells, sizeof before);
    g->score += board_slide(g->cells, dir);

    if (memcmp(before, g->cells, sizeof before) == 0) return 0;
    game_spawn_tile(g, random_below);
    return 1;
}
