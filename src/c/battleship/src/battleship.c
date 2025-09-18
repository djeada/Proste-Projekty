#include "battleship.h"
#include <stdlib.h>
#include <string.h>

static const int SHIP_LENGTHS[MAX_SHIPS] = {5,4,3,3,2};

static void clear_board(Board *b) {
    for (int y = 0; y < BOARD_SIZE; ++y)
        for (int x = 0; x < BOARD_SIZE; ++x)
            b->grid[y][x] = (Cell){0,0};
}

void battleship_init(BattleGame *game, int max_x, int max_y) {
    memset(game, 0, sizeof(*game));
    game->max_x = max_x; game->max_y = max_y;
    clear_board(&game->player);
    clear_board(&game->enemy);
    for (int i = 0; i < MAX_SHIPS; ++i) {
        game->player.ships[i] = (Ship){.pos={0,0}, .length=SHIP_LENGTHS[i], .horizontal=1, .hits=0, .placed=0};
        game->enemy.ships[i]  = (Ship){.pos={0,0}, .length=SHIP_LENGTHS[i], .horizontal=1, .hits=0, .placed=0};
    }
    game->phase = PHASE_PLACEMENT;
    game->cursor_x = 0; game->cursor_y = 0; game->current_ship = 0;
    game->player_ships_remaining = MAX_SHIPS; game->enemy_ships_remaining = MAX_SHIPS;
}

int place_ship(Board *board, int ship_index, int x, int y, int horizontal) {
    if (ship_index < 0 || ship_index >= MAX_SHIPS) return 0;
    int len = board->ships[ship_index].length;
    if (horizontal) {
        if (x + len > BOARD_SIZE) return 0;
        for (int i = 0; i < len; ++i) if (board->grid[y][x+i].has_ship) return 0;
        for (int i = 0; i < len; ++i) board->grid[y][x+i].has_ship = 1;
    } else {
        if (y + len > BOARD_SIZE) return 0;
        for (int i = 0; i < len; ++i) if (board->grid[y+i][x].has_ship) return 0;
        for (int i = 0; i < len; ++i) board->grid[y+i][x].has_ship = 1;
    }
    board->ships[ship_index].pos.x = x; board->ships[ship_index].pos.y = y;
    board->ships[ship_index].horizontal = horizontal; board->ships[ship_index].placed = 1; board->ships[ship_index].hits = 0;
    return 1;
}

int fire_at(Board *board, int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) return 0;
    if (board->grid[y][x].hit) return 0;
    board->grid[y][x].hit = 1;
    if (!board->grid[y][x].has_ship) return 0;
    // Mark hit on corresponding ship
    for (int s = 0; s < MAX_SHIPS; ++s) {
        Ship *ship = &board->ships[s];
        if (!ship->placed) continue;
        for (int i = 0; i < ship->length; ++i) {
            int sx = ship->pos.x + (ship->horizontal ? i : 0);
            int sy = ship->pos.y + (ship->horizontal ? 0 : i);
            if (sx == x && sy == y) {
                ship->hits++;
                return 1;
            }
        }
    }
    return 1;
}

int all_ships_placed(const Board *board) {
    for (int i = 0; i < MAX_SHIPS; ++i) if (!board->ships[i].placed) return 0;
    return 1;
}

int is_defeated(const Board *board) {
    for (int i = 0; i < MAX_SHIPS; ++i) {
        const Ship *s = &board->ships[i];
        if (!s->placed) return 0;
        if (s->hits < s->length) return 0;
    }
    return 1;
}

static void random_place(Board *board) {
    for (int s = 0; s < MAX_SHIPS; ++s) {
        int placed = 0;
        for (int tries = 0; tries < 100 && !placed; ++tries) {
            int horiz = rand() % 2;
            int x = rand() % BOARD_SIZE;
            int y = rand() % BOARD_SIZE;
            placed = place_ship(board, s, x, y, horiz);
        }
    }
}

void battleship_update(BattleGame *game, int key) {
    if (key == 'q') { game->phase = PHASE_BATTLE; game->player_ships_remaining = 0; game->enemy_ships_remaining = 0; return; }

    if (game->phase == PHASE_PLACEMENT) {
        if (key == 'r') {
            Ship *s = &game->player.ships[game->current_ship];
            s->horizontal = !s->horizontal;
        } else if (key == '\n') {
            Ship *s = &game->player.ships[game->current_ship];
            if (place_ship(&game->player, game->current_ship, game->cursor_x, game->cursor_y, s->horizontal)) {
                game->current_ship++;
                if (game->current_ship >= MAX_SHIPS) {
                    // Auto-place enemy and switch to battle
                    random_place(&game->enemy);
                    game->phase = PHASE_BATTLE;
                }
            }
        } else if (key == 'w') { if (game->cursor_y > 0) game->cursor_y--; }
        else if (key == 's') { if (game->cursor_y < BOARD_SIZE - 1) game->cursor_y++; }
        else if (key == 'a') { if (game->cursor_x > 0) game->cursor_x--; }
        else if (key == 'd') { if (game->cursor_x < BOARD_SIZE - 1) game->cursor_x++; }
    } else { // PHASE_BATTLE
        if (key == '\n') {
            fire_at(&game->enemy, game->cursor_x, game->cursor_y);
        } else if (key == 'w') { if (game->cursor_y > 0) game->cursor_y--; }
        else if (key == 's') { if (game->cursor_y < BOARD_SIZE - 1) game->cursor_y++; }
        else if (key == 'a') { if (game->cursor_x > 0) game->cursor_x--; }
        else if (key == 'd') { if (game->cursor_x < BOARD_SIZE - 1) game->cursor_x++; }
    }
}

static void draw_board_text(const Board *b, FILE *out, int show_ships, int cursor_x, int cursor_y) {
    fprintf(out, "   ");
    for (int x = 0; x < BOARD_SIZE; ++x) fprintf(out, "%d ", x);
    fprintf(out, "\n");
    for (int y = 0; y < BOARD_SIZE; ++y) {
        fprintf(out, "%2d ", y);
        for (int x = 0; x < BOARD_SIZE; ++x) {
            char c = '.';
            if (b->grid[y][x].hit) c = b->grid[y][x].has_ship ? 'X' : 'o';
            else if (show_ships && b->grid[y][x].has_ship) c = '#';
            if (x == cursor_x && y == cursor_y) fprintf(out, "[%c]", c);
            else fprintf(out, "%c ", c);
        }
        fprintf(out, "\n");
    }
}

void battleship_draw_text(const BattleGame *game, FILE *out) {
    if (game->phase == PHASE_PLACEMENT) {
        fprintf(out, "Battleship - Placement\n");
        fprintf(out, "Ship %d/%d length %d - Rotate: r, Place: Enter\n", game->current_ship+1, MAX_SHIPS, game->player.ships[game->current_ship].length);
        draw_board_text(&game->player, out, 1, game->cursor_x, game->cursor_y);
    } else {
        fprintf(out, "Battleship - Battle\n");
        fprintf(out, "Fire: Enter | Move: WASD | Quit: q\n");
        draw_board_text(&game->enemy, out, 0, game->cursor_x, game->cursor_y);
    }
}
