#ifndef FIFTEEN_H
#define FIFTEEN_H

#define SIZE 4

typedef struct {
    int board[SIZE][SIZE];
    int empty_row;
    int empty_col;
} FifteenPuzzle;

void puzzle_init(FifteenPuzzle *puzzle);
void puzzle_shuffle(FifteenPuzzle *puzzle);
int puzzle_move(FifteenPuzzle *puzzle, char move);
int puzzle_is_solved(const FifteenPuzzle *puzzle);
void puzzle_print(const FifteenPuzzle *puzzle);

#endif // FIFTEEN_H
