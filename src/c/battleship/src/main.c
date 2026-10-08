/* Terminal Battleship (ncurses): place your fleet, then fire at the computer's board. */
#include <ncurses.h>
#include <stdio.h>
#include <time.h>

#include "battleship.h"

typedef enum { PHASE_PLACING, PHASE_BATTLE, PHASE_OVER } Phase;

typedef struct {
    Board player;
    Board enemy;
    Computer computer;
    Rng rng;
    Phase phase;
    int player_won;
    int horizontal;
    int cursor_x;
    int cursor_y;
    char message[120];
    char reply[120];
} Game;

enum { PAIR_SHIP = 1, PAIR_HIT, PAIR_MISS, PAIR_WATER };
enum { LEFT_BOARD_COL = 4, RIGHT_BOARD_COL = 40, BOARD_ROW = 4 };

static void new_game(Game *g) {
    board_reset(&g->player);
    board_reset(&g->enemy);
    board_place_random(&g->enemy, &g->rng);
    computer_reset(&g->computer);
    g->phase = PHASE_PLACING;
    g->player_won = 0;
    g->horizontal = 1;
    g->cursor_x = 0;
    g->cursor_y = 0;
    g->reply[0] = '\0';
    snprintf(g->message, sizeof g->message, "Place your ships. Next ship: length %d.",
             g->player.ships[0].length);
}

static int next_ship(const Game *g) {
    for (int s = 0; s < SHIP_COUNT; ++s) {
        if (!g->player.ships[s].placed) return s;
    }
    return NO_SHIP;
}

static void move_cursor(Game *g, int dx, int dy) {
    int nx = g->cursor_x + dx;
    int ny = g->cursor_y + dy;
    if (nx >= 0 && nx < BOARD_SIZE) g->cursor_x = nx;
    if (ny >= 0 && ny < BOARD_SIZE) g->cursor_y = ny;
}

static void place_ship(Game *g) {
    int s = next_ship(g);
    if (!board_place(&g->player, s, g->cursor_x, g->cursor_y, g->horizontal)) {
        snprintf(g->message, sizeof g->message, "Cannot place a ship there (off the board or overlapping).");
    } else if (board_fleet_placed(&g->player)) {
        g->phase = PHASE_BATTLE;
        snprintf(g->message, sizeof g->message, "All ships placed. Battle! Fire at the right board.");
    } else {
        snprintf(g->message, sizeof g->message, "Ship placed. Next ship: length %d.",
                 g->player.ships[next_ship(g)].length);
    }
}

static void random_fleet(Game *g) {
    board_place_random(&g->player, &g->rng);
    g->phase = PHASE_BATTLE;
    snprintf(g->message, sizeof g->message, "Random fleet placed. Battle! Fire at the right board.");
}

static void describe_shot(char *out, size_t size, const Board *board, int x, int y, ShotResult r) {
    if (r == SHOT_SUNK) {
        snprintf(out, size, "sunk a ship of length %d", board->ships[board->ship_at[y][x]].length);
    } else {
        snprintf(out, size, "%s", r == SHOT_HIT ? "hit" : "miss");
    }
}

static void fire(Game *g) {
    int x = g->cursor_x;
    int y = g->cursor_y;
    ShotResult r = board_fire(&g->enemy, x, y);
    if (r == SHOT_REPEAT) {
        snprintf(g->message, sizeof g->message, "You already fired at that cell. Choose another one.");
        g->reply[0] = '\0';
        return;
    }
    char mine[48];
    describe_shot(mine, sizeof mine, &g->enemy, x, y, r);
    snprintf(g->message, sizeof g->message, "You fire at %c%d: %s.", 'A' + x, y + 1, mine);
    g->reply[0] = '\0';
    if (board_all_sunk(&g->enemy)) {
        g->phase = PHASE_OVER;
        g->player_won = 1;
        return;
    }
    Point p = computer_choose(&g->computer, &g->player, &g->rng);
    ShotResult cr = board_fire(&g->player, p.x, p.y);
    computer_report(&g->computer, &g->player, p, cr);
    char theirs[48];
    describe_shot(theirs, sizeof theirs, &g->player, p.x, p.y, cr);
    snprintf(g->reply, sizeof g->reply, "Computer fires at %c%d: %s.", 'A' + p.x, p.y + 1, theirs);
    if (board_all_sunk(&g->player)) {
        g->phase = PHASE_OVER;
        g->player_won = 0;
    }
}

/* Returns 0 to quit the game. */
static int handle_key(Game *g, int key) {
    switch (key) {
        case 'q':
        case 'Q':
            return 0;
        case 'n':
        case 'N':
            new_game(g);
            break;
        case 'w': case KEY_UP:    move_cursor(g, 0, -1); break;
        case 's': case KEY_DOWN:  move_cursor(g, 0, 1); break;
        case 'a': case KEY_LEFT:  move_cursor(g, -1, 0); break;
        case 'd': case KEY_RIGHT: move_cursor(g, 1, 0); break;
        case 'r':
        case 'R':
            if (g->phase == PHASE_PLACING) g->horizontal = !g->horizontal;
            break;
        case 'x':
        case 'X':
            if (g->phase == PHASE_PLACING) random_fleet(g);
            break;
        case '\n':
        case KEY_ENTER:
            if (g->phase == PHASE_PLACING) place_ship(g);
            else if (g->phase == PHASE_BATTLE) fire(g);
            break;
        default:
            break;
    }
    return 1;
}

static void draw_board(int top, int left, const Board *board, int show_ships, int active, const Game *g) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
        mvaddch(top - 1, left + 3 + 2 * x, 'A' + x);
    }
    for (int y = 0; y < BOARD_SIZE; ++y) {
        mvprintw(top + y, left, "%2d", y + 1);
        for (int x = 0; x < BOARD_SIZE; ++x) {
            int index = board->ship_at[y][x];
            char ch = '~';
            int pair = PAIR_WATER;
            if (board->shot[y][x] && index == NO_SHIP) {
                ch = 'o';
                pair = PAIR_MISS;
            } else if (board->shot[y][x]) {
                /* a hit; the cells of a sunk ship are drawn as '#' */
                ch = board->ships[index].hits == board->ships[index].length ? '#' : 'X';
                pair = PAIR_HIT;
            } else if (show_ships && index != NO_SHIP) {
                ch = '#';
                pair = PAIR_SHIP;
            }
            int attr = COLOR_PAIR(pair);
            if (active && x == g->cursor_x && y == g->cursor_y) attr |= A_REVERSE;
            attron(attr);
            mvaddch(top + y, left + 3 + 2 * x, ch);
            attroff(attr);
        }
    }
}

static void draw(const Game *g) {
    erase();
    mvprintw(0, LEFT_BOARD_COL, "BATTLESHIP");
    mvprintw(2, LEFT_BOARD_COL, "Your fleet");
    mvprintw(2, RIGHT_BOARD_COL, "Computer's waters");
    draw_board(BOARD_ROW, LEFT_BOARD_COL, &g->player, 1, g->phase == PHASE_PLACING, g);
    draw_board(BOARD_ROW, RIGHT_BOARD_COL, &g->enemy, g->phase == PHASE_OVER, g->phase == PHASE_BATTLE, g);
    if (g->phase == PHASE_OVER) {
        attron(A_BOLD);
        mvprintw(0, RIGHT_BOARD_COL, g->player_won ? "YOU WON!" : "YOU LOST");
        attroff(A_BOLD);
    } else if (g->phase == PHASE_PLACING) {
        mvprintw(BOARD_ROW + BOARD_SIZE + 1, LEFT_BOARD_COL, "Orientation: %s",
                 g->horizontal ? "horizontal" : "vertical");
    }
    mvprintw(BOARD_ROW + BOARD_SIZE + 3, LEFT_BOARD_COL, "%s", g->message);
    mvprintw(BOARD_ROW + BOARD_SIZE + 4, LEFT_BOARD_COL, "%s", g->reply);
    if (g->phase == PHASE_PLACING) {
        mvprintw(BOARD_ROW + BOARD_SIZE + 6, LEFT_BOARD_COL,
                 "WASD/arrows move  R rotate  Enter place  X random fleet  N new  Q quit");
    } else {
        mvprintw(BOARD_ROW + BOARD_SIZE + 6, LEFT_BOARD_COL,
                 "WASD/arrows move  Enter fire  N new game  Q quit");
    }
    refresh();
}

int main(void) {
    Game g;
    rng_seed(&g.rng, (unsigned int)time(NULL));
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    if (has_colors()) {
        start_color();
        init_pair(PAIR_SHIP, COLOR_GREEN, COLOR_BLACK);
        init_pair(PAIR_HIT, COLOR_RED, COLOR_BLACK);
        init_pair(PAIR_MISS, COLOR_CYAN, COLOR_BLACK);
        init_pair(PAIR_WATER, COLOR_BLUE, COLOR_BLACK);
    }
    new_game(&g);
    draw(&g);
    while (handle_key(&g, getch())) {
        draw(&g);
    }
    endwin();
    return 0;
}
