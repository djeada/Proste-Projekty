/* Terminal user interface: draws the field with ncurses and reads the keys. */
#define _POSIX_C_SOURCE 199309L
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "shooting_ducks.h"

#define SKY_PAIR 1
#define GRASS_PAIR 2
#define DUCK_PAIR 3
#define CROSSHAIR_PAIR 4
#define MIN_ROWS 12
#define MIN_COLS 40

static const char *const DUCK_RIGHT[2] = {"  _ ", "(o)>"};
static const char *const DUCK_LEFT[2] = {" _  ", "<(o)"};

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

/* Row 0 is the status bar, the last row is the grass; the rest is the sky. */
static int play_rows(void) { return LINES - 2; }

static int field_to_col(double x) { return (int)(x / FIELD_WIDTH * COLS); }

static int field_to_row(double y) { return 1 + (int)(y / FIELD_HEIGHT * play_rows()); }

static void put_char(int row, int col, char ch) {
    if (row >= 1 && row <= LINES - 2 && col >= 0 && col < COLS) mvaddch(row, col, ch);
}

static void draw_duck(const Duck *duck) {
    const char *const *art = duck->speed > 0 ? DUCK_RIGHT : DUCK_LEFT;
    int top = field_to_row(duck->y) - 1;
    int left = field_to_col(duck->x) - 2;
    attron(COLOR_PAIR(DUCK_PAIR) | A_BOLD);
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (art[r][c] != ' ') put_char(top + r, left + c, art[r][c]);
        }
    }
    attroff(COLOR_PAIR(DUCK_PAIR) | A_BOLD);
}

static void draw_sky(void) {
    attron(COLOR_PAIR(SKY_PAIR));
    for (int row = 1; row <= LINES - 2; ++row) {
        for (int col = 0; col < COLS; ++col) mvaddch(row, col, ' ');
    }
    attroff(COLOR_PAIR(SKY_PAIR));
}

static void draw_grass(void) {
    attron(COLOR_PAIR(GRASS_PAIR));
    mvhline(LINES - 1, 0, ' ', COLS);
    for (int col = 0; col < COLS; col += 3) mvaddch(LINES - 1, col, '"');
    attroff(COLOR_PAIR(GRASS_PAIR));
}

static void draw_centered(int row, const char *text, int attrs) {
    attron(attrs);
    mvprintw(row, (COLS - (int)strlen(text)) / 2, "%s", text);
    attroff(attrs);
}

static void draw_game(const Game *game, int cx, int cy) {
    erase();
    if (LINES < MIN_ROWS || COLS < MIN_COLS) {
        mvprintw(0, 0, "Make the terminal larger (at least %dx%d)", MIN_COLS, MIN_ROWS);
        refresh();
        return;
    }

    draw_sky();
    for (int i = 0; i < game->duck_count; ++i) draw_duck(&game->ducks[i]);
    draw_grass();

    attron(A_REVERSE);
    mvhline(0, 0, ' ', COLS);
    mvprintw(0, 1, "Wave %d   Score %d   Lives %d", game->wave, game->score, game->lives);
    attroff(A_REVERSE);

    int middle = LINES / 2;
    if (game->game_over) {
        draw_centered(middle - 1, "GAME OVER", A_BOLD | A_REVERSE);
        char score[32];
        snprintf(score, sizeof(score), "Score: %d", game->score);
        draw_centered(middle, score, A_BOLD);
        draw_centered(middle + 1, "R - play again   Q - quit", 0);
    } else {
        attron(COLOR_PAIR(CROSSHAIR_PAIR) | A_BOLD);
        mvaddch(cy, cx, '+');
        attroff(COLOR_PAIR(CROSSHAIR_PAIR) | A_BOLD);
        if (game->break_timer > 0) {
            char text[32];
            snprintf(text, sizeof(text), "Wave %d cleared!", game->wave);
            draw_centered(middle, text, A_BOLD);
        }
    }
    refresh();
}

static void move_crosshair(int key, int *cx, int *cy) {
    if (key == KEY_LEFT && *cx > 0) (*cx)--;
    if (key == KEY_RIGHT && *cx < COLS - 1) (*cx)++;
    if (key == KEY_UP && *cy > 1) (*cy)--;
    if (key == KEY_DOWN && *cy < LINES - 2) (*cy)++;
}

int main(void) {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(20);
    if (has_colors()) {
        start_color();
        init_pair(SKY_PAIR, COLOR_WHITE, COLOR_BLUE);
        init_pair(GRASS_PAIR, COLOR_BLACK, COLOR_GREEN);
        init_pair(DUCK_PAIR, COLOR_YELLOW, COLOR_BLUE);
        init_pair(CROSSHAIR_PAIR, COLOR_RED, COLOR_BLUE);
    }

    Game game;
    game_init(&game, (uint32_t)time(NULL));
    int cx = COLS / 2, cy = LINES / 2;
    double last = now_seconds();

    for (;;) {
        double now = now_seconds();
        double dt = now - last;
        last = now;
        if (dt > 0.1) dt = 0.1;
        game_update(&game, dt);
        draw_game(&game, cx, cy);

        int key = getch();
        if (key == 'q' || key == 'Q') break;
        if (key == KEY_LEFT || key == KEY_RIGHT || key == KEY_UP || key == KEY_DOWN) {
            move_crosshair(key, &cx, &cy);
        } else if (key == ' ') {
            /* Cell centre converted back to field coordinates. */
            double x = (cx + 0.5) * FIELD_WIDTH / COLS;
            double y = (cy - 1 + 0.5) * FIELD_HEIGHT / play_rows();
            game_shoot(&game, x, y);
        } else if ((key == 'r' || key == 'R') && game.game_over) {
            game_init(&game, (uint32_t)time(NULL));
        }
    }

    endwin();
    return 0;
}
