#include "zombie_apocalypse.h"
#include <stdio.h>

int main() {
    ZombieApocalypse game;
    zombie_apocalypse_init(&game);
    printf("Zombie Apocalypse begins!\n");
    while (!zombie_apocalypse_is_over(&game)) {
        printf("Player health: %d, Zombies left: %d\n", game.player_health, game.zombie_count);
        zombie_apocalypse_tick(&game);
    }
    if (game.player_health > 0) {
        printf("You survived the zombie apocalypse!\n");
    } else {
        printf("You were eaten by zombies...\n");
    }
    return 0;
}
