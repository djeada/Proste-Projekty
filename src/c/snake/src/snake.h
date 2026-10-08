/* The rules of Snake: no input or output here. */
#ifndef SNAKE_H
#define SNAKE_H

#define SNAKE_WIDTH 20
#define SNAKE_HEIGHT 15
#define SNAKE_MAX_CELLS (SNAKE_WIDTH * SNAKE_HEIGHT)

typedef struct {
    int x, y;
} Cell;

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

/* Returns a random number in [0, n). Tests pass a fake one. */
typedef int (*RandomFn)(int n);

typedef struct {
    Cell body[SNAKE_MAX_CELLS]; /* body[0] is the head */
    int length;
    Direction direction;      /* the direction of the last step */
    Direction next_direction; /* the direction of the next step */
    Cell food;
    int score;
    int game_over;
} SnakeGame;

void game_init(SnakeGame *game, RandomFn random_below);
void game_turn(SnakeGame *game, Direction direction);
void game_step(SnakeGame *game, RandomFn random_below);
int game_delay_ms(const SnakeGame *game);

#endif /* SNAKE_H */
