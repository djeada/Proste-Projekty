/* Rules of tic-tac-toe and the minimax search for the computer player. */
#include "tic_tac_toe.h"

#include <stddef.h>

static const int LINES[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6},
};

Board board_new(void)
{
    Board board = {{EMPTY}}; /* EMPTY is 0, so the remaining cells are empty too */
    return board;
}

Mark other_mark(Mark mark)
{
    return mark == X ? O : X;
}

bool board_play(Board *board, int cell, Mark mark)
{
    if (cell < 0 || cell > 8 || board->cells[cell] != EMPTY) {
        return false;
    }
    board->cells[cell] = mark;
    return true;
}

int board_legal_moves(const Board *board, int moves[9])
{
    int count = 0;
    int cell;
    for (cell = 0; cell < 9; cell++) {
        if (board->cells[cell] == EMPTY) {
            moves[count++] = cell;
        }
    }
    return count;
}

const int *board_winning_line(const Board *board)
{
    int i;
    for (i = 0; i < 8; i++) {
        const int *line = LINES[i];
        Mark first = board->cells[line[0]];
        if (first != EMPTY && first == board->cells[line[1]] && first == board->cells[line[2]]) {
            return line;
        }
    }
    return NULL;
}

Mark board_winner(const Board *board)
{
    const int *line = board_winning_line(board);
    return line ? board->cells[line[0]] : EMPTY;
}

bool board_is_full(const Board *board)
{
    int moves[9];
    return board_legal_moves(board, moves) == 0;
}

bool board_is_over(const Board *board)
{
    return board_winning_line(board) != NULL || board_is_full(board);
}

bool board_is_draw(const Board *board)
{
    return board_winning_line(board) == NULL && board_is_full(board);
}

/* Score for `me` when `turn` is to move: +10 for a win, -10 for a loss and 0 for a draw.
 * The depth is subtracted from a win, so the quickest win scores highest and the slowest
 * loss scores least badly. */
static int minimax(const Board *board, Mark turn, Mark me, int depth)
{
    const int *line = board_winning_line(board);
    int moves[9];
    int count = board_legal_moves(board, moves);
    int best;
    int i;

    if (line) {
        return board->cells[line[0]] == me ? 10 - depth : depth - 10;
    }
    if (count == 0) {
        return 0;
    }

    best = turn == me ? -100 : 100;
    for (i = 0; i < count; i++) {
        Board next = *board;
        int score;
        next.cells[moves[i]] = turn;
        score = minimax(&next, other_mark(turn), me, depth + 1);
        if (turn == me ? score > best : score < best) {
            best = score;
        }
    }
    return best;
}

int board_best_move(const Board *board, Mark mark)
{
    int moves[9];
    int count = board_legal_moves(board, moves);
    int best_cell = -1;
    int best_score = -100;
    int i;

    for (i = 0; i < count; i++) {
        Board next = *board;
        int score;
        next.cells[moves[i]] = mark;
        score = minimax(&next, other_mark(mark), mark, 1);
        if (best_cell < 0 || score > best_score) {
            best_score = score;
            best_cell = moves[i];
        }
    }
    return best_cell;
}
