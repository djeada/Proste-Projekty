#ifndef ZOMBIE_APOCALYPSE_H
#define ZOMBIE_APOCALYPSE_H

#include <ncurses.h>

#define MAX_ZOMBIES 32
#define MAX_BULLETS 16
#define MAX_PARTICLES 64
#define PLAYER_START_HEALTH 5
// Progressive spawn settings
#define BASE_ZOMBIES 6
#define ZOMBIES_PER_LEVEL 4

// Visual effect durations (in ticks)
#define DAMAGE_FLASH_DURATION 4
#define EXPLOSION_DURATION 6
#define LEVEL_TRANSITION_DURATION 15

// Game area margins for HUD
#define HUD_HEIGHT 4
#define BORDER_WIDTH 1

// Color pairs
#define COLOR_PLAYER 1
#define COLOR_ZOMBIE 2
#define COLOR_BULLET 3
#define COLOR_HUD 4
#define COLOR_BORDER 5
#define COLOR_HEALTH_HIGH 6
#define COLOR_HEALTH_MED 7
#define COLOR_HEALTH_LOW 8
#define COLOR_EXPLOSION 9
#define COLOR_WAVE_CLEAR 10
#define COLOR_GAME_OVER 11
#define COLOR_PARTICLE 12
#define COLOR_TITLE 13

// Directions
typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE } Direction;

// Particle types for visual effects
typedef enum {
    PARTICLE_NONE,
    PARTICLE_EXPLOSION,
    PARTICLE_SPARK,
    PARTICLE_DEBRIS
} ParticleType;

typedef struct {
    int x, y;
    int health;
    int invincible_ticks; // brief invincibility after damage
} Player;

typedef struct {
    int x, y;
    int alive;
    int death_anim; // ticks remaining for death animation
} Zombie;

typedef struct {
    int x, y;
    Direction dir;
    int active;
    int trail_tick; // for bullet trail effect
} Bullet;

typedef struct {
    int x, y;
    ParticleType type;
    int lifetime;
    char symbol;
    int color_pair;
} Particle;

typedef struct {
    Player player;
    Zombie zombies[MAX_ZOMBIES];
    int zombie_count;
    Bullet bullets[MAX_BULLETS];
    Particle particles[MAX_PARTICLES];
    int max_x, max_y;
    int game_area_x, game_area_y; // top-left of playable area
    int game_area_w, game_area_h; // playable area size
    int game_over; // 1 when player dies
    int wave_cleared; // 1 when all zombies are dead
    int level; // starts at 1
    int score;
    int high_score;
    int tick; // global tick for timing
    int zombie_move_period; // lower = faster zombies
    Direction last_dir; // last movement direction (for shooting)
    int paused; // 1 when paused
    // Visual effects state
    int damage_flash; // ticks remaining for damage flash
    int level_transition; // ticks remaining for level transition effect
    int screen_shake; // ticks remaining for screen shake
    int shake_offset_x, shake_offset_y; // current shake offset
} ZombieGame;

void zombie_game_init(ZombieGame *game, int max_x, int max_y);
void zombie_game_update(ZombieGame *game, int key);
void zombie_game_draw(const ZombieGame *game);
void zombie_game_next_level(ZombieGame *game);
void zombie_game_init_colors(void);

#endif // ZOMBIE_APOCALYPSE_H
