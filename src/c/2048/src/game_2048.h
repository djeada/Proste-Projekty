#ifndef GAME_2048_H
#define GAME_2048_H

#include <stdio.h>

#define G2048_SIZE 4

typedef struct {
    int cells[G2048_SIZE][G2048_SIZE];
    int score;
    int game_over;
} G2048;

typedef enum { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT } G2048Move;

void g2048_init(G2048 *g);
void g2048_seed_random(unsigned int seed);
void g2048_draw_text(const G2048 *g, FILE *out);
int g2048_can_move(const G2048 *g);
int g2048_move(G2048 *g, G2048Move dir); // returns 1 if any tile moved/merged
void g2048_spawn_random(G2048 *g);       // spawns 2 (90%) or 4 (10%) on empty cell

// Helpers for tests
void g2048_set(G2048 *g, int row, int col, int val);
int g2048_get(const G2048 *g, int row, int col);

#endif // GAME_2048_H
