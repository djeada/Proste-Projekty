/* Tests of the 2048 rules. A scripted random source makes every spawn predictable. */
#include "game_2048.h"

#include <assert.h>
#include <string.h>

#define N G2048_SIZE

static int script[8];
static int script_len;
static int script_pos;
static int random_calls;

static int scripted_random(int n) {
    assert(script_pos < script_len);
    int v = script[script_pos++];
    assert(v >= 0 && v < n);
    random_calls++;
    return v;
}

/* Each spawn uses two draws: the index of the empty cell, then the roll (0 gives a 4). */
static void use_script(const int *values, int len) {
    memcpy(script, values, len * sizeof(int));
    script_len = len;
    script_pos = 0;
}

static void test_slide_row_classic_cases(void) {
    int a[N] = {2, 2, 2, 2};
    assert(slide_row(a) == 8);
    assert(a[0] == 4 && a[1] == 4 && a[2] == 0 && a[3] == 0);

    /* The two 4s merge into 8; that 8 must not merge with the other 8. */
    int b[N] = {4, 4, 8, 0};
    assert(slide_row(b) == 8);
    assert(b[0] == 8 && b[1] == 8 && b[2] == 0 && b[3] == 0);

    /* Gaps are closed before merging. */
    int c[N] = {2, 0, 2, 4};
    assert(slide_row(c) == 4);
    assert(c[0] == 4 && c[1] == 4 && c[2] == 0 && c[3] == 0);
}

static void test_slide_row_without_merging(void) {
    int a[N] = {0, 0, 0, 2};
    assert(slide_row(a) == 0);
    assert(a[0] == 2 && a[1] == 0 && a[2] == 0 && a[3] == 0);

    int b[N] = {2, 4, 8, 16};
    assert(slide_row(b) == 0);
    assert(b[0] == 2 && b[1] == 4 && b[2] == 8 && b[3] == 16);
}

static void test_board_slide_in_all_directions(void) {
    int start[N][N] = {
        {2, 0, 0, 0},
        {2, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 4},
    };
    int up[N][N];
    memcpy(up, start, sizeof up);
    assert(board_slide(up, DIR_UP) == 4);
    assert(up[0][0] == 4 && up[1][0] == 0 && up[2][0] == 0 && up[3][0] == 0);
    assert(up[0][3] == 4 && up[3][3] == 0);

    int down[N][N];
    memcpy(down, start, sizeof down);
    assert(board_slide(down, DIR_DOWN) == 4);
    assert(down[3][0] == 4 && down[0][0] == 0 && down[1][0] == 0 && down[2][0] == 0);
    assert(down[3][3] == 4 && down[0][3] == 0);

    int right[N][N];
    memcpy(right, start, sizeof right);
    assert(board_slide(right, DIR_RIGHT) == 0);
    assert(right[0][3] == 2 && right[1][3] == 2 && right[3][3] == 4);
    assert(right[0][0] == 0 && right[3][0] == 0);

    int left[N][N];
    memcpy(left, start, sizeof left);
    assert(board_slide(left, DIR_LEFT) == 0);
    assert(left[0][0] == 2 && left[1][0] == 2 && left[3][0] == 4);
    assert(left[0][3] == 0);
}

static void test_move_that_changes_nothing_spawns_nothing(void) {
    Game g;
    memset(&g, 0, sizeof g);
    g.cells[0][0] = 2;
    g.cells[0][1] = 4;
    random_calls = 0;
    script_len = 0;
    assert(game_move(&g, DIR_LEFT, scripted_random) == 0);
    assert(random_calls == 0);
    assert(g.cells[0][0] == 2 && g.cells[0][1] == 4 && g.score == 0);
}

static void test_move_merges_and_spawns_one_tile(void) {
    Game g;
    memset(&g, 0, sizeof g);
    g.cells[0][0] = 2;
    g.cells[0][1] = 2;
    const int values[] = {0, 1}; /* first empty cell (0,1); roll 1 gives a 2 */
    use_script(values, 2);
    assert(game_move(&g, DIR_LEFT, scripted_random) == 1);
    assert(g.score == 4);
    assert(g.cells[0][0] == 4);
    assert(g.cells[0][1] == 2);
    assert(g.cells[0][2] == 0 && g.cells[0][3] == 0);
    assert(random_calls == 2);
}

static void test_spawn_is_two_or_four(void) {
    Game g;
    memset(&g, 0, sizeof g);
    const int four[] = {0, 0}; /* roll 0 gives a 4 */
    use_script(four, 2);
    assert(game_spawn_tile(&g, scripted_random) == 1);
    assert(g.cells[0][0] == 4);

    memset(&g, 0, sizeof g);
    const int two[] = {0, 5}; /* roll 5 gives a 2 */
    use_script(two, 2);
    assert(game_spawn_tile(&g, scripted_random) == 1);
    assert(g.cells[0][0] == 2);
}

static void test_spawn_on_full_board_does_nothing(void) {
    Game g;
    memset(&g, 0, sizeof g);
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) g.cells[r][c] = 2;
    }
    random_calls = 0;
    script_len = 0;
    assert(game_spawn_tile(&g, scripted_random) == 0);
    assert(random_calls == 0);
}

static void test_start_puts_two_tiles(void) {
    Game g;
    const int values[] = {0, 0, 0, 5}; /* first tile: 4 at (0,0); second: 2 at (0,1) */
    use_script(values, 4);
    game_start(&g, scripted_random);
    assert(g.cells[0][0] == 4);
    assert(g.cells[0][1] == 2);
    assert(g.score == 0);
    assert(random_calls == 4);
}

static void test_full_board_without_merges_is_lost(void) {
    Game g;
    memset(&g, 0, sizeof g);
    int values[N][N] = {
        {2, 4, 2, 4},
        {4, 2, 4, 2},
        {2, 4, 2, 4},
        {4, 2, 4, 2},
    };
    memcpy(g.cells, values, sizeof values);
    assert(!game_can_move(&g));
    assert(game_status(&g) == STATUS_LOST);

    /* Two equal neighbours in row 0 make a move possible again. */
    g.cells[0][1] = 2;
    assert(game_can_move(&g));
    assert(game_status(&g) == STATUS_PLAYING);
}

static void test_reaching_2048_wins_until_player_continues(void) {
    Game g;
    memset(&g, 0, sizeof g);
    g.cells[0][0] = 1024;
    g.cells[0][1] = 1024;
    assert(game_status(&g) == STATUS_PLAYING);

    const int values[] = {0, 5}; /* spawn after the winning move: a 2 at (0,1) */
    use_script(values, 2);
    assert(game_move(&g, DIR_LEFT, scripted_random) == 1);
    assert(g.cells[0][0] == 2048);
    assert(game_status(&g) == STATUS_WON);

    /* After winning, moves are ignored until the player chooses to continue. */
    random_calls = 0;
    script_len = 0;
    assert(game_move(&g, DIR_RIGHT, scripted_random) == 0);
    assert(random_calls == 0);

    game_keep_playing(&g);
    assert(game_status(&g) == STATUS_PLAYING);
}

int main(void) {
    test_slide_row_classic_cases();
    test_slide_row_without_merging();
    test_board_slide_in_all_directions();
    test_move_that_changes_nothing_spawns_nothing();
    test_move_merges_and_spawns_one_tile();
    test_spawn_is_two_or_four();
    test_spawn_on_full_board_does_nothing();
    test_start_puts_two_tiles();
    test_full_board_without_merges_is_lost();
    test_reaching_2048_wins_until_player_continues();
    return 0;
}
