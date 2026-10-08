/* Terminal interface: choose a level, type commands r ROW COL, f ROW COL or q. */
#include "minesweeper.h"

#include <stdio.h>
#include <time.h>

typedef struct {
    const char *name;
    int rows;
    int cols;
    int mines;
} Level;

static const Level levels[] = {
    {"Beginner", 9, 9, 10},
    {"Intermediate", 16, 16, 40},
    {"Expert", 16, 30, 99},
};

/* Returns the index of the chosen level, or -1 when the input ends. */
static int choose_level(void) {
    char line[64];
    int choice = 0;
    for (;;) {
        printf("Choose a level: 1 Beginner (9x9, 10 mines), 2 Intermediate (16x16, 40 mines), "
               "3 Expert (16x30, 99 mines): ");
        fflush(stdout);
        if (!fgets(line, sizeof line, stdin)) {
            return -1;
        }
        if (sscanf(line, "%d", &choice) == 1 && choice >= 1 && choice <= 3) {
            return choice - 1;
        }
    }
}

static char cell_symbol(const Game *game, int row, int col) {
    if (game->revealed[row][col]) {
        if (game->mine[row][col]) {
            return '*';
        }
        if (game->neighbors[row][col] == 0) {
            return ' ';
        }
        return (char)('0' + game->neighbors[row][col]);
    }
    return game->flagged[row][col] ? 'F' : '.';
}

/* ANSI codes: clear the whole screen and move the cursor to the top left corner. */
static void clear_screen(void) {
    printf("\033[2J\033[H");
}

static void print_board(const Game *game) {
    /* Column numbers are printed as two lines, tens and units, so they fit in 2 characters each. */
    printf("    ");
    for (int col = 0; col < game->cols; col++) {
        printf(" %c", col >= 10 ? '0' + col / 10 : ' ');
    }
    printf("\n    ");
    for (int col = 0; col < game->cols; col++) {
        printf(" %d", col % 10);
    }
    printf("\n");
    for (int row = 0; row < game->rows; row++) {
        printf("%2d  ", row);
        for (int col = 0; col < game->cols; col++) {
            printf(" %c", cell_symbol(game, row, col));
        }
        printf("\n");
    }
}

int main(void) {
    int choice = choose_level();
    if (choice < 0) {
        return 0;
    }
    const Level *level = &levels[choice];
    Game game;
    time_t start = time(NULL);
    game_start(&game, level->rows, level->cols, level->mines, (unsigned int)start);

    char line[64];
    while (game.state == PLAYING) {
        clear_screen();
        printf("Mines left: %d   Time: %ld s\n", game_mines_left(&game),
               (long)(time(NULL) - start));
        print_board(&game);
        printf("> ");
        fflush(stdout);
        if (!fgets(line, sizeof line, stdin)) {
            break;
        }

        char cmd = 0;
        int row = 0;
        int col = 0;
        int fields = sscanf(line, " %c %d %d", &cmd, &row, &col);
        if (cmd == 'q') {
            break;
        }
        if (fields != 3 || (cmd != 'r' && cmd != 'f')) {
            printf("Commands: r ROW COL (reveal), f ROW COL (flag), q (quit)\n");
            continue;
        }
        if (row < 0 || row >= level->rows || col < 0 || col >= level->cols) {
            printf("Row must be 0-%d and column 0-%d\n", level->rows - 1, level->cols - 1);
            continue;
        }
        if (cmd == 'r') {
            game_reveal(&game, row, col);
        } else {
            game_toggle_flag(&game, row, col);
        }
    }

    clear_screen();
    print_board(&game);
    if (game.state == WON) {
        printf("You won! Every safe cell is open.\n");
    } else if (game.state == LOST) {
        printf("Boom! You hit a mine.\n");
    } else {
        printf("Game abandoned.\n");
    }
    return 0;
}
