/* The terminal interface of Zombie Apocalypse: reads keys, draws with ncurses. */
#define _POSIX_C_SOURCE 199309L /* clock_gettime */

#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <time.h>

#include "zombie_apocalypse.h"

#define MIN_COLS 64
#define MIN_ROWS 24
#define FIELD_TOP 2   /* screen row of world row 0 */
#define FIELD_LEFT 1  /* screen column of world column 0 */
#define HOLD_SECONDS 0.15 /* a movement key keeps the player walking for this long */
#define KEY_ESC 27

#define PAIR_PLAYER 1
#define PAIR_ZOMBIE 2
#define PAIR_BULLET 3
#define PAIR_PICKUP 4

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static int cell(double value, int size) {
    int c = (int)floor(value);
    if (c < 0) {
        return 0;
    }
    return c >= size ? size - 1 : c;
}

static void put(Vec2 pos, char symbol, int pair, int style) {
    attron(COLOR_PAIR(pair) | style);
    mvaddch(FIELD_TOP + cell(pos.y, WORLD_HEIGHT), FIELD_LEFT + cell(pos.x, WORLD_WIDTH), symbol);
    attroff(COLOR_PAIR(pair) | style);
}

static void draw_border(void) {
    int top = FIELD_TOP - 1;
    int bottom = FIELD_TOP + WORLD_HEIGHT;
    for (int x = 0; x <= WORLD_WIDTH + 1; x++) {
        mvaddch(top, x, '-');
        mvaddch(bottom, x, '-');
    }
    for (int y = top + 1; y < bottom; y++) {
        mvaddch(y, 0, '|');
        mvaddch(y, WORLD_WIDTH + 1, '|');
    }
}

static void draw_game(const Game *game) {
    erase();
    mvprintw(0, 0, "Wave %d   Score %d   Health %d   Zombies left %d", game->wave, game->score,
             game->health, game->to_spawn + game->zombie_count);
    draw_border();
    for (int i = 0; i < game->pickup_count; i++) {
        put(game->pickups[i], '+', PAIR_PICKUP, 0);
    }
    for (int i = 0; i < game->zombie_count; i++) {
        put(game->zombies[i], 'Z', PAIR_ZOMBIE, A_BOLD);
    }
    for (int i = 0; i < game->bullet_count; i++) {
        put(game->bullets[i].pos, '*', PAIR_BULLET, 0);
    }
    put(game->player, '@', PAIR_PLAYER, 0);

    int message_row = FIELD_TOP + WORLD_HEIGHT + 1;
    if (game->game_over) {
        mvprintw(message_row, 0, "GAME OVER - press R to play again or Q to quit");
    } else if (game->wave_delay > 0.0) {
        mvprintw(message_row, 0, "Wave %d cleared! The next wave is coming...", game->wave);
    } else {
        mvprintw(message_row, 0, "Move: WASD or arrows   Shoot: Space   Quit: Q");
    }
    refresh();
}

static void init_colors(void) {
    if (!has_colors()) {
        return;
    }
    start_color();
    use_default_colors();
    init_pair(PAIR_PLAYER, COLOR_CYAN, -1);
    init_pair(PAIR_ZOMBIE, COLOR_GREEN, -1);
    init_pair(PAIR_BULLET, COLOR_YELLOW, -1);
    init_pair(PAIR_PICKUP, COLOR_RED, -1);
}

int main(void) {
    initscr();
    if (LINES < MIN_ROWS || COLS < MIN_COLS) {
        endwin();
        fprintf(stderr, "The terminal is too small: need at least %dx%d\n", MIN_COLS, MIN_ROWS);
        return 1;
    }
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    init_colors();

    Game game;
    game_init(&game, (uint32_t)time(NULL));
    Vec2 held = {0.0, 0.0};
    double held_until = 0.0;
    double last = now_seconds();
    int running = 1;

    while (running) {
        Input input = {{0.0, 0.0}, {0.0, 0.0}, 0};
        int key;
        while ((key = getch()) != ERR) {
            switch (key) {
            case 'w':
            case KEY_UP:
                input.move.y -= 1.0;
                break;
            case 's':
            case KEY_DOWN:
                input.move.y += 1.0;
                break;
            case 'a':
            case KEY_LEFT:
                input.move.x -= 1.0;
                break;
            case 'd':
            case KEY_RIGHT:
                input.move.x += 1.0;
                break;
            case ' ':
                input.fire = 1; /* aim zero: the bullet flies in the last movement direction */
                break;
            case 'r':
                if (game.game_over) {
                    game_init(&game, (uint32_t)time(NULL));
                }
                break;
            case 'q':
            case KEY_ESC:
                running = 0;
                break;
            default:
                break;
            }
        }

        double now = now_seconds();
        double dt = now - last > 0.1 ? 0.1 : now - last;
        last = now;
        /* Terminals send key repeats with gaps, so a movement key keeps working for a moment. */
        if (input.move.x != 0.0 || input.move.y != 0.0) {
            held = input.move;
            held_until = now + HOLD_SECONDS;
        }
        Vec2 none = {0.0, 0.0};
        input.move = now < held_until ? held : none;

        game_update(&game, &input, dt);
        draw_game(&game);
        napms(15);
    }

    endwin();
    printf("Thanks for playing! Score: %d, waves survived: %d\n", game.score, game.wave - 1);
    return 0;
}
