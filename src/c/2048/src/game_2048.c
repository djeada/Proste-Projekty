#include "game_2048.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static unsigned int rng_seeded = 0;

void g2048_seed_random(unsigned int seed) {
    srand(seed);
    rng_seeded = 1;
}

static int random_int(int n) { return rand() % n; }

void g2048_init(G2048 *g) {
    memset(g, 0, sizeof(*g));
    if (!rng_seeded) g2048_seed_random((unsigned)time(NULL));
    g2048_spawn_random(g);
    g2048_spawn_random(g);
}

void g2048_set(G2048 *g, int row, int col, int val) {
    if (row < 0 || row >= G2048_SIZE || col < 0 || col >= G2048_SIZE) return;
    g->cells[row][col] = val;
}

int g2048_get(const G2048 *g, int row, int col) {
    if (row < 0 || row >= G2048_SIZE || col < 0 || col >= G2048_SIZE) return 0;
    return g->cells[row][col];
}

static void slide_and_merge_line(int *line, int len, int *score, int *moved) {
    int tmp[4] = {0};
    int t = 0;
    for (int i = 0; i < len; ++i) if (line[i] != 0) tmp[t++] = line[i];
    int out[4] = {0};
    int o = 0;
    for (int i = 0; i < t; ++i) {
        if (i+1 < t && tmp[i] == tmp[i+1]) {
            out[o++] = tmp[i] * 2;
            *score += tmp[i] * 2;
            i++;
        } else {
            out[o++] = tmp[i];
        }
    }
    for (int i = 0; i < len; ++i) {
        if (line[i] != out[i]) *moved = 1;
        line[i] = out[i];
    }
}

int g2048_move(G2048 *g, G2048Move dir) {
    int moved = 0;
    if (dir == MOVE_LEFT) {
        for (int r = 0; r < G2048_SIZE; ++r) {
            int line[4];
            for (int c = 0; c < G2048_SIZE; ++c) line[c] = g->cells[r][c];
            slide_and_merge_line(line, G2048_SIZE, &g->score, &moved);
            for (int c = 0; c < G2048_SIZE; ++c) g->cells[r][c] = line[c];
        }
    } else if (dir == MOVE_RIGHT) {
        for (int r = 0; r < G2048_SIZE; ++r) {
            int line[4];
            for (int c = 0; c < G2048_SIZE; ++c) line[c] = g->cells[r][G2048_SIZE-1-c];
            slide_and_merge_line(line, G2048_SIZE, &g->score, &moved);
            for (int c = 0; c < G2048_SIZE; ++c) g->cells[r][G2048_SIZE-1-c] = line[c];
        }
    } else if (dir == MOVE_UP) {
        for (int c = 0; c < G2048_SIZE; ++c) {
            int line[4];
            for (int r = 0; r < G2048_SIZE; ++r) line[r] = g->cells[r][c];
            slide_and_merge_line(line, G2048_SIZE, &g->score, &moved);
            for (int r = 0; r < G2048_SIZE; ++r) g->cells[r][c] = line[r];
        }
    } else if (dir == MOVE_DOWN) {
        for (int c = 0; c < G2048_SIZE; ++c) {
            int line[4];
            for (int r = 0; r < G2048_SIZE; ++r) line[r] = g->cells[G2048_SIZE-1-r][c];
            slide_and_merge_line(line, G2048_SIZE, &g->score, &moved);
            for (int r = 0; r < G2048_SIZE; ++r) g->cells[G2048_SIZE-1-r][c] = line[r];
        }
    }
    if (moved) g2048_spawn_random(g);
    if (!g2048_can_move(g)) g->game_over = 1;
    return moved;
}

void g2048_spawn_random(G2048 *g) {
    int empties[16][2]; int n = 0;
    for (int r = 0; r < G2048_SIZE; ++r) for (int c = 0; c < G2048_SIZE; ++c)
        if (g->cells[r][c] == 0) { empties[n][0] = r; empties[n][1] = c; n++; }
    if (n == 0) return;
    int idx = random_int(n);
    int val = (random_int(10) == 0) ? 4 : 2;
    g->cells[empties[idx][0]][empties[idx][1]] = val;
}

int g2048_can_move(const G2048 *g) {
    for (int r = 0; r < G2048_SIZE; ++r) {
        for (int c = 0; c < G2048_SIZE; ++c) {
            int v = g->cells[r][c];
            if (v == 0) return 1;
            if (r+1 < G2048_SIZE && g->cells[r+1][c] == v) return 1;
            if (c+1 < G2048_SIZE && g->cells[r][c+1] == v) return 1;
        }
    }
    return 0;
}

void g2048_draw_text(const G2048 *g, FILE *out) {
    fprintf(out, "\033[2J\033[H");
    fprintf(out, "2048 - WASD move, q quit\n\n");
    fprintf(out, "Score: %d\n\n", g->score);
    for (int r = 0; r < G2048_SIZE; ++r) {
        for (int c = 0; c < G2048_SIZE; ++c) {
            if (g->cells[r][c] == 0) fprintf(out, ".   ");
            else fprintf(out, "%4d", g->cells[r][c]);
        }
        fprintf(out, "\n");
    }
    if (g->game_over) fprintf(out, "\nGame Over! Press r to restart or q to quit.\n");
}
