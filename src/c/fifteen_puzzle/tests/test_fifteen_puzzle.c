/* Tests of the fifteen puzzle rules. Returns 0 when all tests pass. */
#undef NDEBUG  /* keep assert() active in every build type */
#include <assert.h>
#include <stdio.h>

#include "fifteen_puzzle.h"

static void set_tiles(Board *board, const int tiles[CELLS]) {
    for (int i = 0; i < CELLS; i++) {
        board->tiles[i] = tiles[i];
    }
}

static void test_solved_board(void) {
    Board board;
    board_reset(&board);
    assert(board.tiles[0] == 1);
    assert(board.tiles[14] == 15);
    assert(board.tiles[15] == 0);
    assert(board_gap(&board) == 15);
    assert(board_is_solved(&board));
    assert(board_is_solvable(&board));
}

static void test_slide_adjacent_only(void) {
    Board board;
    board_reset(&board);
    assert(!board_slide(&board, 10));  /* diagonal to the gap */
    assert(board_slide(&board, 11));   /* tile 12 is above the gap */
    assert(board.tiles[15] == 12 && board.tiles[11] == 0);
    assert(!board_slide(&board, 11));  /* the gap itself */
    assert(!board_slide(&board, 3));   /* far away from the gap */
    assert(!board_slide(&board, 16));  /* outside the board */
}

static void test_move_gap_at_edges(void) {
    Board board;
    board_reset(&board);
    assert(!board_move_gap(&board, DIR_DOWN));   /* gap is in the bottom row */
    assert(!board_move_gap(&board, DIR_RIGHT));  /* gap is in the right column */
    assert(board_move_gap(&board, DIR_UP));
    assert(board_gap(&board) == 11);
    assert(board.tiles[15] == 12);
    assert(board_move_gap(&board, DIR_LEFT));
    assert(board_gap(&board) == 10);
    assert(board.tiles[11] == 11);
}

static void test_solved_after_undo(void) {
    Board board;
    board_reset(&board);
    assert(board_move_gap(&board, DIR_LEFT));
    assert(!board_is_solved(&board));
    assert(board_move_gap(&board, DIR_RIGHT));
    assert(board_is_solved(&board));
}

static void test_solvable_after_moves(void) {
    Board board;
    board_reset(&board);
    board_move_gap(&board, DIR_UP);
    board_move_gap(&board, DIR_LEFT);
    assert(board_is_solvable(&board));
}

static void test_unsolvable_swap(void) {
    /* Swapping tiles 14 and 15 is the classic impossible position. */
    Board board;
    int tiles[CELLS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 14, 0};
    set_tiles(&board, tiles);
    assert(!board_is_solvable(&board));
    assert(!board_is_solved(&board));
}

static void test_shuffle_is_solvable_and_seeded(void) {
    Board first, second, board;
    Rng rng;
    for (unsigned int seed = 1; seed <= 50; seed++) {
        rng_seed(&rng, seed);
        board_shuffle(&board, &rng);
        assert(board_is_solvable(&board));
        assert(!board_is_solved(&board));
    }
    rng_seed(&rng, 42);
    board_shuffle(&first, &rng);
    rng_seed(&rng, 42);
    board_shuffle(&second, &rng);
    for (int i = 0; i < CELLS; i++) {
        assert(first.tiles[i] == second.tiles[i]);
    }
}

static void test_shuffle_keeps_all_tiles(void) {
    Board board;
    Rng rng;
    int seen[CELLS] = {0};
    rng_seed(&rng, 7);
    board_shuffle(&board, &rng);
    for (int i = 0; i < CELLS; i++) {
        seen[board.tiles[i]]++;
    }
    for (int tile = 0; tile < CELLS; tile++) {
        assert(seen[tile] == 1);
    }
}

int main(void) {
    test_solved_board();
    test_slide_adjacent_only();
    test_move_gap_at_edges();
    test_solved_after_undo();
    test_solvable_after_moves();
    test_unsolvable_swap();
    test_shuffle_is_solvable_and_seeded();
    test_shuffle_keeps_all_tiles();
    printf("All 8 fifteen puzzle tests passed.\n");
    return 0;
}
