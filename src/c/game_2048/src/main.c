/* Terminal user interface: draws the board with ANSI colors and reads the keys. */
#include "game_2048.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define N G2048_SIZE
#define TILE_WIDTH 7

static struct termios saved_termios;

static void restore_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
}

/* Keys are read one at a time, without waiting for Enter. */
static void enable_single_key_input(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &saved_termios);
    atexit(restore_terminal);
    raw = saved_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

/* Arrow keys are returned as the matching W, A, S, D keys. */
static int read_key(void) {
    int ch = getchar();
    if (ch == EOF) return 'q';
    if (ch != '\033') return ch;
    if (getchar() != '[') return 0;
    switch (getchar()) {
        case 'A': return 'w';
        case 'B': return 's';
        case 'C': return 'd';
        case 'D': return 'a';
        default: return 0;
    }
}

static int random_below(int n) {
    return rand() % n;
}

static int tile_background(int value) {
    static const int palette[] = {230, 229, 223, 216, 209, 203, 227, 221, 215, 208, 202, 196};
    if (value == 0) return 250;
    int index = 0;
    for (int v = value; v > 2 && index < 11; v /= 2) index++;
    return palette[index];
}

static int tile_text(int value) {
    return value <= 4 ? 236 : 231;
}

/* Each tile is three lines high: the value is drawn on the middle line. */
static void draw_board(const Game *g) {
    printf("\033[H\033[2J");
    printf("\033[1;36m  2048\033[0m   Score: \033[1;33m%d\033[0m\n\n", g->score);

    for (int r = 0; r < N; r++) {
        for (int line = 0; line < 3; line++) {
            printf("  ");
            for (int c = 0; c < N; c++) {
                int v = g->cells[r][c];
                printf("\033[48;5;%d;38;5;%dm", tile_background(v), tile_text(v));
                if (line == 1 && v != 0) {
                    char digits[8];
                    snprintf(digits, sizeof digits, "%d", v);
                    int len = (int)strlen(digits);
                    int left = (TILE_WIDTH - len) / 2;
                    printf("%*s%s%*s", left, "", digits, TILE_WIDTH - len - left, "");
                } else {
                    printf("%*s", TILE_WIDTH, "");
                }
                printf("\033[0m ");
            }
            printf("\n");
        }
        if (r < N - 1) printf("\n");
    }

    printf("\n");
    switch (game_status(g)) {
        case STATUS_WON:
            printf("\033[1;32mYou reached 2048! Press C to keep playing or R for a new game.\033[0m\n");
            break;
        case STATUS_LOST:
            printf("\033[1;31mGame over! Press R to play again or Q to quit.\033[0m\n");
            break;
        case STATUS_PLAYING:
            printf("Arrows or W A S D: move   R: restart   Q: quit\n");
            break;
    }
    fflush(stdout);
}

int main(void) {
    srand((unsigned)time(NULL));
    enable_single_key_input();

    Game g;
    game_start(&g, random_below);
    for (;;) {
        draw_board(&g);
        int key = read_key();
        if (key == 'q') break;
        if (key == 'r') {
            game_start(&g, random_below);
        } else if (key == 'c') {
            game_keep_playing(&g);
        } else if (key == 'w') {
            game_move(&g, DIR_UP, random_below);
        } else if (key == 's') {
            game_move(&g, DIR_DOWN, random_below);
        } else if (key == 'a') {
            game_move(&g, DIR_LEFT, random_below);
        } else if (key == 'd') {
            game_move(&g, DIR_RIGHT, random_below);
        }
    }

    printf("\033[H\033[2J");
    printf("Thanks for playing 2048! Final score: %d\n", g.score);
    return 0;
}
