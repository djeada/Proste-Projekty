#ifndef ZOMBIE_APOCALYPSE_H
#define ZOMBIE_APOCALYPSE_H

typedef struct {
    int player_health;
    int zombie_count;
} ZombieApocalypse;

void zombie_apocalypse_init(ZombieApocalypse *game);
void zombie_apocalypse_tick(ZombieApocalypse *game);
int zombie_apocalypse_is_over(const ZombieApocalypse *game);

#endif // ZOMBIE_APOCALYPSE_H
