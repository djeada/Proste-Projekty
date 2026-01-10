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

static const char *get_tile_color(int val) {
    switch (val) {
        case 2:    return "\033[48;5;230m\033[38;5;236m";  /* Light cream bg, dark text */
        case 4:    return "\033[48;5;223m\033[38;5;236m";  /* Tan bg, dark text */
        case 8:    return "\033[48;5;209m\033[38;5;231m";  /* Orange bg, white text */
        case 16:   return "\033[48;5;203m\033[38;5;231m";  /* Light red bg, white text */
        case 32:   return "\033[48;5;196m\033[38;5;231m";  /* Red bg, white text */
        case 64:   return "\033[48;5;160m\033[38;5;231m";  /* Dark red bg, white text */
        case 128:  return "\033[48;5;226m\033[38;5;236m";  /* Yellow bg, dark text */
        case 256:  return "\033[48;5;220m\033[38;5;236m";  /* Gold bg, dark text */
        case 512:  return "\033[48;5;214m\033[38;5;231m";  /* Orange-gold bg, white text */
        case 1024: return "\033[48;5;208m\033[38;5;231m";  /* Deep orange bg, white text */
        case 2048: return "\033[48;5;202m\033[38;5;231m";  /* Bright orange bg, white text */
        default:   return "\033[48;5;198m\033[38;5;231m";  /* Pink bg for >2048, white text */
    }
}

static void print_tile(FILE *out, int val) {
    if (val == 0) {
        fprintf(out, "\033[48;5;250m\033[38;5;250m");  /* Gray background for empty */
        fprintf(out, "  .   ");
    } else {
        fprintf(out, "%s", get_tile_color(val));
        fprintf(out, "%6d", val);
    }
    fprintf(out, "\033[0m");  /* Reset colors */
}

void g2048_draw_text(const G2048 *g, FILE *out) {
    fprintf(out, "\033[2J\033[H");  /* Clear screen, move cursor to top */
    
    /* Header */
    fprintf(out, "\033[1;36m");  /* Bold cyan */
    fprintf(out, "╔════════════════════════════════════╗\n");
    fprintf(out, "║             \033[1;33m2 0 4 8\033[1;36m               ║\n");
    fprintf(out, "╠════════════════════════════════════╣\n");
    fprintf(out, "║  \033[0;37mControls: W/A/S/D or Arrow Keys\033[1;36m   ║\n");
    fprintf(out, "║  \033[0;37mQuit: Q  |  Restart: R\033[1;36m            ║\n");
    fprintf(out, "╠════════════════════════════════════╣\n");
    fprintf(out, "║  \033[1;32mScore: %-10d\033[1;36m                ║\n", g->score);
    fprintf(out, "╚════════════════════════════════════╝\033[0m\n\n");
    
    /* Top border of grid */
    fprintf(out, "\033[1;35m┌────────┬────────┬────────┬────────┐\033[0m\n");
    
    /* Grid rows */
    for (int r = 0; r < G2048_SIZE; ++r) {
        fprintf(out, "\033[1;35m│\033[0m");
        for (int c = 0; c < G2048_SIZE; ++c) {
            fprintf(out, " ");
            print_tile(out, g->cells[r][c]);
            fprintf(out, " \033[1;35m│\033[0m");
        }
        fprintf(out, "\n");
        
        /* Row separator or bottom border */
        if (r < G2048_SIZE - 1) {
            fprintf(out, "\033[1;35m├────────┼────────┼────────┼────────┤\033[0m\n");
        } else {
            fprintf(out, "\033[1;35m└────────┴────────┴────────┴────────┘\033[0m\n");
        }
    }
    
    /* Game over message */
    if (g->game_over) {
        fprintf(out, "\n\033[1;31m");  /* Bold red */
        fprintf(out, "╔════════════════════════════════════╗\n");
        fprintf(out, "║           \033[5mGAME OVER!\033[0;1;31m              ║\n");
        fprintf(out, "║   Press R to restart or Q to quit  ║\n");
        fprintf(out, "╚════════════════════════════════════╝\033[0m\n");
    }
    
    fflush(out);
}
