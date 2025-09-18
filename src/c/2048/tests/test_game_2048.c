#include "game_2048.h"
#include <assert.h>
#include <stdio.h>

static void test_merge_left_simple() {
    G2048 g; g2048_seed_random(1); g2048_init(&g);
    // override known state
    for (int r=0;r<G2048_SIZE;r++) for (int c=0;c<G2048_SIZE;c++) g2048_set(&g,r,c,0);
    g2048_set(&g,0,0,2); g2048_set(&g,0,1,2); g2048_set(&g,0,2,4); g2048_set(&g,0,3,0);
    int moved = g2048_move(&g, MOVE_LEFT);
    assert(moved == 1);
    assert(g2048_get(&g,0,0) == 4);
    assert(g2048_get(&g,0,1) == 4);
    // ensure spawn happened in an empty spot
    int empties = 0; for (int r=0;r<G2048_SIZE;r++) for (int c=0;c<G2048_SIZE;c++) if (g2048_get(&g,r,c)==0) empties++;
    assert(empties == 13);
}

static void test_no_move_no_spawn() {
    G2048 g; g2048_seed_random(1); g2048_init(&g);
    for (int r=0;r<G2048_SIZE;r++) for (int c=0;c<G2048_SIZE;c++) g2048_set(&g,r,c,0);
    g2048_set(&g,0,0,2); g2048_set(&g,0,1,4); g2048_set(&g,0,2,8); g2048_set(&g,0,3,16);
    int moved = g2048_move(&g, MOVE_RIGHT);
    assert(moved == 0);
    // no spawn if nothing moved
    int count_nonzero = 0; for (int r=0;r<G2048_SIZE;r++) for (int c=0;c<G2048_SIZE;c++) if (g2048_get(&g,r,c)!=0) count_nonzero++;
    assert(count_nonzero == 4);
}

static void test_can_move_detection() {
    G2048 g; g2048_seed_random(1); g2048_init(&g);
    // Fill with checkerboard unmergeable
    int vals[2]={2,4};
    for (int r=0;r<G2048_SIZE;r++) for (int c=0;c<G2048_SIZE;c++) g2048_set(&g,r,c, vals[(r+c)&1]);
    assert(g2048_can_move(&g) == 0);
}

int main(){
    test_merge_left_simple();
    test_no_move_no_spawn();
    test_can_move_detection();
    printf("All 2048 tests passed!\n");
    return 0;
}
