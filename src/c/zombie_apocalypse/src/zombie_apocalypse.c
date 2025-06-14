#include "zombie_apocalypse.h"
#include <stdio.h>

#define PLAYER_START_HEALTH 100
#define INITIAL_ZOMBIE_COUNT 10
#define ZOMBIE_ATTACK_DAMAGE 10

// TODO: Implement the main logic for the zombie apocalypse simulation/game.

void zombie_apocalypse_init(ZombieApocalypse *game) {
    // Initialize game state
    game->player_health = PLAYER_START_HEALTH;
    game->zombie_count = INITIAL_ZOMBIE_COUNT;
}

void zombie_apocalypse_tick(ZombieApocalypse *game) {
    // Example: Each tick, a zombie attacks
    if (game->zombie_count > 0) {
        game->player_health -= ZOMBIE_ATTACK_DAMAGE;
        game->zombie_count--;
    }
}

int zombie_apocalypse_is_over(const ZombieApocalypse *game) {
    return game->player_health <= 0 || game->zombie_count == 0;
}
