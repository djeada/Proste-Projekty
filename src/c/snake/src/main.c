/* Terminal user interface for Snake, drawn with ncurses. */
#define _POSIX_C_SOURCE 199309L

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "snake.h"

/* Screen position of board cell (x, y): the border is at row 1 and column 0. */
#define SCREEN_ROW(y) ((y) + 2)
#define SCREEN_COL(x) ((x) + 1)

static int random_below(int n) {
    return rand() % n;
}

static long now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

static void draw_board(void) {
    for (int y = 0; y < SNAKE_HEIGHT + 2; y++) {
        for (int x = 0; x < SNAKE_WIDTH + 2; x++) {
            if (y == 0 || y == SNAKE_HEIGHT + 1 || x == 0 || x == SNAKE_WIDTH + 1) {
                mvaddch(y + 1, x, '#');
            }
        }
    }
}

static void draw(const SnakeGame *game, int paused) {
    erase();
    mvprintw(0, 0, "Score: %d", game->score);
    draw_board();
    mvaddch(SCREEN_ROW(game->food.y), SCREEN_COL(game->food.x), '*');
    for (int i = 0; i < game->length; i++) {
        mvaddch(SCREEN_ROW(game->body[i].y), SCREEN_COL(game->body[i].x), i == 0 ? 'O' : 'o');
    }
    if (game->game_over) {
        mvprintw(SNAKE_HEIGHT + 4, 0, "Game over! Press R or Space to restart, Q to quit.");
    } else if (paused) {
        mvprintw(SNAKE_HEIGHT + 4, 0, "Paused. Press P to continue.");
    } else {
        mvprintw(SNAKE_HEIGHT + 4, 0, "Arrows or WASD: move  P: pause  Q: quit");
    }
    refresh();
}

/* Returns 0 when the player wants to quit. */
static int handle_key(int key, SnakeGame *game, int *paused) {
    switch (key) {
        case KEY_UP:
        case 'w':
        case 'W':
            game_turn(game, UP);
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            game_turn(game, DOWN);
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
            game_turn(game, LEFT);
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            game_turn(game, RIGHT);
            break;
        case 'p':
        case 'P':
            *paused = !*paused;
            break;
        case 'r':
        case 'R':
        case ' ':
            if (game->game_over) {
                game_init(game, random_below);
                *paused = 0;
            }
            break;
        case 'q':
        case 'Q':
            return 0;
        default:
            break;
    }
    return 1;
}

int main(void) {
    srand((unsigned)time(NULL));
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    if (LINES < SNAKE_HEIGHT + 5 || COLS < SNAKE_WIDTH + 2) {
        endwin();
        fprintf(stderr, "The terminal must be at least %dx%d.\n", SNAKE_WIDTH + 2, SNAKE_HEIGHT + 5);
        return 1;
    }

    SnakeGame game;
    game_init(&game, random_below);
    int paused = 0;
    long next_step = now_ms() + game_delay_ms(&game);
    int running = 1;

    while (running) {
        draw(&game, paused);
        long wait = next_step - now_ms();
        timeout(wait > 0 ? (int)wait : 0);
        int key = getch();
        if (key == ERR) {
            /* The time for the next step has come. */
            if (!paused) {
                game_step(&game, random_below);
            }
            next_step = now_ms() + game_delay_ms(&game);
        } else {
            running = handle_key(key, &game, &paused);
        }
    }

    endwin();
    return 0;
}
