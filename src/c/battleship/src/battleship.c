/* Battleship rules: boards, ship placement, shooting and the computer player. */
#include "battleship.h"

static const int FLEET_LENGTHS[SHIP_COUNT] = {5, 4, 3, 3, 2};
static const int DX[4] = {1, -1, 0, 0};
static const int DY[4] = {0, 0, 1, -1};

static int in_bounds(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

static int ship_sunk(const Ship *ship) {
    return ship->placed && ship->hits == ship->length;
}

void rng_seed(Rng *rng, unsigned int seed) {
    rng->state = seed ? seed : 1u;
}

static unsigned int rng_next(Rng *rng) {
    unsigned int x = rng->state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng->state = x;
    return x;
}

int rng_below(Rng *rng, int n) {
    return (int)(rng_next(rng) % (unsigned int)n);
}

void board_reset(Board *board) {
    for (int s = 0; s < SHIP_COUNT; ++s) {
        board->ships[s].length = FLEET_LENGTHS[s];
        board->ships[s].hits = 0;
        board->ships[s].placed = 0;
    }
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            board->ship_at[y][x] = NO_SHIP;
            board->shot[y][x] = 0;
        }
    }
}

int board_can_place(const Board *board, int ship, int x, int y, int horizontal) {
    if (ship < 0 || ship >= SHIP_COUNT || board->ships[ship].placed) return 0;
    for (int i = 0; i < board->ships[ship].length; ++i) {
        int cx = horizontal ? x + i : x;
        int cy = horizontal ? y : y + i;
        if (!in_bounds(cx, cy) || board->ship_at[cy][cx] != NO_SHIP) return 0;
    }
    return 1;
}

int board_place(Board *board, int ship, int x, int y, int horizontal) {
    if (!board_can_place(board, ship, x, y, horizontal)) return 0;
    for (int i = 0; i < board->ships[ship].length; ++i) {
        int cx = horizontal ? x + i : x;
        int cy = horizontal ? y : y + i;
        board->ship_at[cy][cx] = ship;
    }
    board->ships[ship].placed = 1;
    return 1;
}

static int place_one_randomly(Board *board, int ship, Rng *rng) {
    for (int tries = 0; tries < 1000; ++tries) {
        int x = rng_below(rng, BOARD_SIZE);
        int y = rng_below(rng, BOARD_SIZE);
        int horizontal = rng_below(rng, 2);
        if (board_place(board, ship, x, y, horizontal)) return 1;
    }
    return 0;
}

/* Replaces the whole fleet with a random valid one. Ships may touch but not overlap. */
void board_place_random(Board *board, Rng *rng) {
    int placed = 0;
    while (!placed) {
        board_reset(board);
        placed = 1;
        for (int s = 0; s < SHIP_COUNT && placed; ++s) {
            placed = place_one_randomly(board, s, rng);
        }
    }
}

int board_fleet_placed(const Board *board) {
    for (int s = 0; s < SHIP_COUNT; ++s) {
        if (!board->ships[s].placed) return 0;
    }
    return 1;
}

ShotResult board_fire(Board *board, int x, int y) {
    if (!in_bounds(x, y)) return SHOT_INVALID;
    if (board->shot[y][x]) return SHOT_REPEAT;
    board->shot[y][x] = 1;
    int index = board->ship_at[y][x];
    if (index == NO_SHIP) return SHOT_MISS;
    Ship *ship = &board->ships[index];
    ship->hits++;
    return ship_sunk(ship) ? SHOT_SUNK : SHOT_HIT;
}

int board_all_sunk(const Board *board) {
    for (int s = 0; s < SHIP_COUNT; ++s) {
        if (!ship_sunk(&board->ships[s])) return 0;
    }
    return 1;
}

void computer_reset(Computer *computer) {
    computer->count = 0;
}

/* Hunt and target: random shots until a hit, then the neighbours of the hit. */
Point computer_choose(Computer *computer, const Board *target, Rng *rng) {
    while (computer->count > 0) {
        Point p = computer->targets[--computer->count];
        if (!target->shot[p.y][p.x]) return p;
    }
    Point unshot[BOARD_SIZE * BOARD_SIZE];
    int n = 0;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (!target->shot[y][x]) unshot[n++] = (Point){x, y};
        }
    }
    return unshot[rng_below(rng, n)];
}

void computer_report(Computer *computer, const Board *target, Point at, ShotResult result) {
    if (result == SHOT_SUNK) {
        computer->count = 0;
        return;
    }
    if (result != SHOT_HIT) return;
    for (int d = 0; d < 4; ++d) {
        int nx = at.x + DX[d];
        int ny = at.y + DY[d];
        if (in_bounds(nx, ny) && !target->shot[ny][nx] && computer->count < MAX_TARGETS) {
            computer->targets[computer->count++] = (Point){nx, ny};
        }
    }
}
