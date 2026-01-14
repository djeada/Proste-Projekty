#include "zombie_apocalypse.h"
#include <stdlib.h>
#include <string.h>

// --- Enhanced ncurses-based ZombieGame with visual effects ---

// Initialize color pairs for the game
void zombie_game_init_colors(void) {
    if (has_colors()) {
        start_color();
        use_default_colors();
        
        // Define color pairs
        init_pair(COLOR_PLAYER, COLOR_CYAN, -1);
        init_pair(COLOR_ZOMBIE, COLOR_GREEN, -1);
        init_pair(COLOR_BULLET, COLOR_YELLOW, -1);
        init_pair(COLOR_HUD, COLOR_WHITE, -1);
        init_pair(COLOR_BORDER, COLOR_BLUE, -1);
        init_pair(COLOR_HEALTH_HIGH, COLOR_GREEN, -1);
        init_pair(COLOR_HEALTH_MED, COLOR_YELLOW, -1);
        init_pair(COLOR_HEALTH_LOW, COLOR_RED, -1);
        init_pair(COLOR_EXPLOSION, COLOR_RED, -1);
        init_pair(COLOR_WAVE_CLEAR, COLOR_CYAN, -1);
        init_pair(COLOR_GAME_OVER, COLOR_RED, -1);
        init_pair(COLOR_PARTICLE, COLOR_MAGENTA, -1);
        init_pair(COLOR_TITLE, COLOR_YELLOW, -1);
    }
}

// Spawn particles for explosion effect
static void spawn_explosion(ZombieGame *game, int x, int y) {
    const char explosion_chars[] = "*+x.,'";
    int spawned = 0;
    for (int i = 0; i < MAX_PARTICLES && spawned < 8; ++i) {
        if (game->particles[i].type == PARTICLE_NONE) {
            game->particles[i].x = x + (rand() % 3) - 1;
            game->particles[i].y = y + (rand() % 3) - 1;
            game->particles[i].type = PARTICLE_EXPLOSION;
            game->particles[i].lifetime = EXPLOSION_DURATION + (rand() % 3);
            game->particles[i].symbol = explosion_chars[rand() % 6];
            game->particles[i].color_pair = (rand() % 2) ? COLOR_EXPLOSION : COLOR_PARTICLE;
            spawned++;
        }
    }
}

// Update particles
static void update_particles(ZombieGame *game) {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (game->particles[i].type != PARTICLE_NONE) {
            game->particles[i].lifetime--;
            if (game->particles[i].lifetime <= 0) {
                game->particles[i].type = PARTICLE_NONE;
            }
        }
    }
}

// Update visual effects (flash, shake, transitions)
static void update_effects(ZombieGame *game) {
    if (game->damage_flash > 0) game->damage_flash--;
    if (game->level_transition > 0) game->level_transition--;
    if (game->screen_shake > 0) {
        game->screen_shake--;
        game->shake_offset_x = (rand() % 3) - 1;
        game->shake_offset_y = (rand() % 3) - 1;
    } else {
        game->shake_offset_x = 0;
        game->shake_offset_y = 0;
    }
    if (game->player.invincible_ticks > 0) game->player.invincible_ticks--;
    
    update_particles(game);
}

static void spawn_zombies(ZombieGame *game) {
    int target = BASE_ZOMBIES + (game->level - 1) * ZOMBIES_PER_LEVEL;
    if (target > MAX_ZOMBIES) target = MAX_ZOMBIES;
    
    // Calculate playable area bounds
    int min_x = game->game_area_x;
    int max_x = game->game_area_x + game->game_area_w;
    int min_y = game->game_area_y;
    int max_y = game->game_area_y + game->game_area_h;
    
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (i < target) {
            // Spawn zombies away from player (at edges)
            // Guard against zero dimensions
            int w = (game->game_area_w > 0) ? game->game_area_w : 1;
            int h = (game->game_area_h > 0) ? game->game_area_h : 1;
            int edge = rand() % 4;
            switch (edge) {
                case 0: // top
                    game->zombies[i].x = min_x + rand() % w;
                    game->zombies[i].y = min_y;
                    break;
                case 1: // bottom
                    game->zombies[i].x = min_x + rand() % w;
                    game->zombies[i].y = max_y - 1;
                    break;
                case 2: // left
                    game->zombies[i].x = min_x;
                    game->zombies[i].y = min_y + rand() % h;
                    break;
                case 3: // right
                    game->zombies[i].x = max_x - 1;
                    game->zombies[i].y = min_y + rand() % h;
                    break;
            }
            game->zombies[i].alive = 1;
            game->zombies[i].death_anim = 0;
        } else {
            game->zombies[i].alive = 0;
            game->zombies[i].death_anim = 0;
        }
    }
    game->zombie_count = target;
}

void zombie_game_init(ZombieGame *game, int max_x, int max_y) {
    int old_high_score = game->high_score;
    memset(game, 0, sizeof(ZombieGame));
    game->high_score = old_high_score;
    game->max_x = max_x;
    game->max_y = max_y;
    
    // Calculate game area with borders and HUD
    game->game_area_x = BORDER_WIDTH;
    game->game_area_y = HUD_HEIGHT + BORDER_WIDTH;
    game->game_area_w = max_x - (2 * BORDER_WIDTH);
    game->game_area_h = max_y - HUD_HEIGHT - (2 * BORDER_WIDTH);
    
    // Center player in game area
    game->player.x = game->game_area_x + game->game_area_w / 2;
    game->player.y = game->game_area_y + game->game_area_h / 2;
    game->player.health = PLAYER_START_HEALTH;
    game->player.invincible_ticks = 0;
    
    game->level = 1;
    spawn_zombies(game);
    for (int i = 0; i < MAX_BULLETS; ++i) {
        game->bullets[i].active = 0;
        game->bullets[i].trail_tick = 0;
    }
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        game->particles[i].type = PARTICLE_NONE;
    }
    game->game_over = 0;
    game->wave_cleared = 0;
    game->score = 0;
    game->tick = 0;
    game->zombie_move_period = 3;
    game->last_dir = DIR_RIGHT;
    game->paused = 0;
    game->damage_flash = 0;
    game->level_transition = LEVEL_TRANSITION_DURATION;
    game->screen_shake = 0;
    game->shake_offset_x = 0;
    game->shake_offset_y = 0;
}

static void move_player(ZombieGame *game, int key) {
    Player *player = &game->player;
    int min_x = game->game_area_x;
    int max_x = game->game_area_x + game->game_area_w - 1;
    int min_y = game->game_area_y;
    int max_y = game->game_area_y + game->game_area_h - 1;
    
    switch (key) {
        case KEY_UP:
        case 'w':
            if (player->y > min_y) { player->y--; }
            break;
        case KEY_DOWN:
        case 's':
            if (player->y < max_y) { player->y++; }
            break;
        case KEY_LEFT:
        case 'a':
            if (player->x > min_x) { player->x--; }
            break;
        case KEY_RIGHT:
        case 'd':
            if (player->x < max_x) { player->x++; }
            break;
        default:
            break;
    }
}

static void shoot_bullet(ZombieGame *game, Direction dir) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!game->bullets[i].active) {
            game->bullets[i].x = game->player.x;
            game->bullets[i].y = game->player.y;
            game->bullets[i].dir = dir;
            game->bullets[i].active = 1;
            game->bullets[i].trail_tick = 0;
            break;
        }
    }
}

static void update_bullets(ZombieGame *game) {
    int min_x = game->game_area_x;
    int max_x = game->game_area_x + game->game_area_w;
    int min_y = game->game_area_y;
    int max_y = game->game_area_y + game->game_area_h;
    
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!game->bullets[i].active) { continue; }
        game->bullets[i].trail_tick++;
        switch (game->bullets[i].dir) {
            case DIR_UP:    game->bullets[i].y--; break;
            case DIR_DOWN:  game->bullets[i].y++; break;
            case DIR_LEFT:  game->bullets[i].x--; break;
            case DIR_RIGHT: game->bullets[i].x++; break;
            default: break;
        }
        if (game->bullets[i].x < min_x || game->bullets[i].x >= max_x ||
            game->bullets[i].y < min_y || game->bullets[i].y >= max_y) {
            game->bullets[i].active = 0;
        }
    }
}

static void update_zombies(ZombieGame *game) {
    if (game->zombie_move_period <= 0) game->zombie_move_period = 1;
    if ((game->tick % game->zombie_move_period) != 0) return;
    
    int min_x = game->game_area_x;
    int max_x = game->game_area_x + game->game_area_w - 1;
    int min_y = game->game_area_y;
    int max_y = game->game_area_y + game->game_area_h - 1;
    
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (!game->zombies[i].alive) { continue; }
        int dx = game->player.x - game->zombies[i].x;
        int dy = game->player.y - game->zombies[i].y;
        if (abs(dx) > abs(dy)) {
            game->zombies[i].x += (dx > 0) ? 1 : -1;
        } else if (dy != 0) {
            game->zombies[i].y += (dy > 0) ? 1 : -1;
        }
        if (game->zombies[i].x < min_x) game->zombies[i].x = min_x;
        if (game->zombies[i].x > max_x) game->zombies[i].x = max_x;
        if (game->zombies[i].y < min_y) game->zombies[i].y = min_y;
        if (game->zombies[i].y > max_y) game->zombies[i].y = max_y;
    }
}

static void handle_collisions(ZombieGame *game) {
    // Bullets vs zombies
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!game->bullets[i].active) { continue; }
        for (int j = 0; j < MAX_ZOMBIES; ++j) {
            if (game->zombies[j].alive &&
                game->bullets[i].x == game->zombies[j].x &&
                game->bullets[i].y == game->zombies[j].y) {
                // Spawn explosion effect
                spawn_explosion(game, game->zombies[j].x, game->zombies[j].y);
                game->zombies[j].alive = 0;
                game->zombies[j].death_anim = EXPLOSION_DURATION;
                game->bullets[i].active = 0;
                game->score += 10;
                break;
            }
        }
    }
    // Zombies vs player (with invincibility check)
    if (game->player.invincible_ticks > 0) return;
    
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game->zombies[i].alive &&
            game->zombies[i].x == game->player.x &&
            game->zombies[i].y == game->player.y) {
            game->player.health--;
            game->zombies[i].alive = 0;
            game->zombies[i].death_anim = EXPLOSION_DURATION;
            spawn_explosion(game, game->zombies[i].x, game->zombies[i].y);
            
            // Trigger damage effects
            game->damage_flash = DAMAGE_FLASH_DURATION;
            game->screen_shake = DAMAGE_FLASH_DURATION;
            game->player.invincible_ticks = 10; // Brief invincibility
            
            if (game->player.health <= 0) {
                game->game_over = 1;
                if (game->score > game->high_score) {
                    game->high_score = game->score;
                }
            }
        }
    }
}

void zombie_game_update(ZombieGame *game, int key) {
    // Always update visual effects
    update_effects(game);
    
    // Update death animations
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game->zombies[i].death_anim > 0) {
            game->zombies[i].death_anim--;
        }
    }
    
    if (game->game_over) { 
        if (key == 'r') {
            zombie_game_init(game, game->max_x, game->max_y);
        }
        return; 
    }

    // Pause 'p', restart 'r', next level 'n'
    if (key == 'r') {
        zombie_game_init(game, game->max_x, game->max_y);
        return;
    }
    if (key == 'p') {
        game->paused = !game->paused;
    }

    if (game->wave_cleared) {
        if (key == 'n') {
            zombie_game_next_level(game);
        }
        return;
    }

    if (game->paused) {
        return;
    }

    // Movement and shooting
    switch (key) {
        case KEY_UP: case 'w': 
            game->last_dir = DIR_UP;  
            move_player(game, key); 
            break;
        case KEY_DOWN: case 's': 
            game->last_dir = DIR_DOWN; 
            move_player(game, key); 
            break;
        case KEY_LEFT: case 'a': 
            game->last_dir = DIR_LEFT; 
            move_player(game, key); 
            break;
        case KEY_RIGHT: case 'd': 
            game->last_dir = DIR_RIGHT; 
            move_player(game, key); 
            break;
        case ' ':
            shoot_bullet(game, game->last_dir);
            break;
        default: break;
    }

    handle_collisions(game);
    update_bullets(game);
    update_zombies(game);
    game->tick++;

    // Check if all zombies are dead
    int any_alive = 0;
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game->zombies[i].alive) { any_alive = 1; break; }
    }
    if (!any_alive) {
        game->wave_cleared = 1;
        game->score += 100;
    }
}

// Draw decorative border around game area
static void draw_border(const ZombieGame *game) {
    int sx = game->shake_offset_x;
    int sy = game->shake_offset_y;
    
    attron(COLOR_PAIR(COLOR_BORDER));
    
    // Top border
    for (int x = 0; x < game->max_x; ++x) {
        mvaddch(HUD_HEIGHT + sy, x + sx, ACS_HLINE);
    }
    // Bottom border
    for (int x = 0; x < game->max_x; ++x) {
        mvaddch(game->max_y - 1 + sy, x + sx, ACS_HLINE);
    }
    // Left border
    for (int y = HUD_HEIGHT; y < game->max_y; ++y) {
        mvaddch(y + sy, sx, ACS_VLINE);
    }
    // Right border
    for (int y = HUD_HEIGHT; y < game->max_y; ++y) {
        mvaddch(y + sy, game->max_x - 1 + sx, ACS_VLINE);
    }
    // Corners
    mvaddch(HUD_HEIGHT + sy, sx, ACS_ULCORNER);
    mvaddch(HUD_HEIGHT + sy, game->max_x - 1 + sx, ACS_URCORNER);
    mvaddch(game->max_y - 1 + sy, sx, ACS_LLCORNER);
    mvaddch(game->max_y - 1 + sy, game->max_x - 1 + sx, ACS_LRCORNER);
    
    attroff(COLOR_PAIR(COLOR_BORDER));
}

// Draw health bar with visual indicators
static void draw_health_bar(const ZombieGame *game, int y, int x) {
    int health = game->player.health;
    int max_health = PLAYER_START_HEALTH;
    
    mvprintw(y, x, "HP: ");
    
    int color;
    if (health > max_health * 2 / 3) {
        color = COLOR_HEALTH_HIGH;
    } else if (health > max_health / 3) {
        color = COLOR_HEALTH_MED;
    } else {
        color = COLOR_HEALTH_LOW;
    }
    
    attron(COLOR_PAIR(color) | A_BOLD);
    for (int i = 0; i < health; ++i) {
        addch(ACS_DIAMOND);
    }
    attroff(COLOR_PAIR(color) | A_BOLD);
    
    attron(COLOR_PAIR(COLOR_HUD) | A_DIM);
    for (int i = health; i < max_health; ++i) {
        addch('.');
    }
    attroff(COLOR_PAIR(COLOR_HUD) | A_DIM);
}

// Draw stylized HUD
static void draw_hud(const ZombieGame *game) {
    int alive = 0;
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        if (game->zombies[i].alive) alive++;
    }
    
    // Title bar
    attron(COLOR_PAIR(COLOR_TITLE) | A_BOLD);
    mvprintw(0, 2, "=== ZOMBIE APOCALYPSE ===");
    attroff(COLOR_PAIR(COLOR_TITLE) | A_BOLD);
    
    // Stats line
    draw_health_bar(game, 1, 2);
    
    attron(COLOR_PAIR(COLOR_HUD));
    mvprintw(1, 20, "Lvl: %d", game->level);
    mvprintw(1, 30, "Score: %d", game->score);
    if (game->high_score > 0) {
        mvprintw(1, 45, "Hi: %d", game->high_score);
    }
    attroff(COLOR_PAIR(COLOR_HUD));
    
    // Zombie counter with pulsing effect when many zombies
    int zombie_color = (alive > game->zombie_count / 2) ? COLOR_ZOMBIE : COLOR_HUD;
    int attr = (alive > game->zombie_count * 3 / 4 && game->tick % 4 < 2) ? A_BOLD : 0;
    attron(COLOR_PAIR(zombie_color) | attr);
    mvprintw(2, 2, "Zombies: %d/%d", alive, game->zombie_count);
    attroff(COLOR_PAIR(zombie_color) | attr);
    
    // Speed indicator
    attron(COLOR_PAIR(COLOR_HUD));
    mvprintw(2, 20, "Speed: ");
    int speed_bars = 4 - game->zombie_move_period + 1;
    if (speed_bars < 1) speed_bars = 1;
    if (speed_bars > 4) speed_bars = 4;
    for (int i = 0; i < speed_bars; ++i) {
        attron(A_BOLD);
        addch('>');
        attroff(A_BOLD);
    }
    attroff(COLOR_PAIR(COLOR_HUD));
    
    // Controls hint
    attron(COLOR_PAIR(COLOR_HUD) | A_DIM);
    mvprintw(3, 2, "[WASD/Arrows] Move  [Space] Shoot  [P] Pause  [R] Restart");
    attroff(COLOR_PAIR(COLOR_HUD) | A_DIM);
}

// Get player character based on direction
static char get_player_char(Direction dir) {
    switch (dir) {
        case DIR_UP:    return '^';
        case DIR_DOWN:  return 'v';
        case DIR_LEFT:  return '<';
        case DIR_RIGHT: return '>';
        default:        return '@';
    }
}

void zombie_game_draw(const ZombieGame *game) {
    int sx = game->shake_offset_x;
    int sy = game->shake_offset_y;
    
    // Apply damage flash effect (red background)
    if (game->damage_flash > 0) {
        attron(COLOR_PAIR(COLOR_EXPLOSION));
        for (int y = HUD_HEIGHT; y < game->max_y; ++y) {
            for (int x = 0; x < game->max_x; ++x) {
                mvaddch(y, x, ' ');
            }
        }
        attroff(COLOR_PAIR(COLOR_EXPLOSION));
    }
    
    // Draw border
    draw_border(game);
    
    // Draw HUD
    draw_hud(game);
    
    // Draw particles (explosions, debris)
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (game->particles[i].type != PARTICLE_NONE) {
            int px = game->particles[i].x + sx;
            int py = game->particles[i].y + sy;
            if (px >= game->game_area_x && px < game->game_area_x + game->game_area_w &&
                py >= game->game_area_y && py < game->game_area_y + game->game_area_h) {
                attron(COLOR_PAIR(game->particles[i].color_pair) | A_BOLD);
                mvaddch(py, px, game->particles[i].symbol);
                attroff(COLOR_PAIR(game->particles[i].color_pair) | A_BOLD);
            }
        }
    }
    
    // Draw zombies with death animation
    for (int i = 0; i < MAX_ZOMBIES; ++i) {
        int zx = game->zombies[i].x + sx;
        int zy = game->zombies[i].y + sy;
        
        if (game->zombies[i].alive) {
            // Animate zombie appearance
            char zombie_char = 'Z';
            if (game->tick % 8 < 4) {
                zombie_char = 'Z';
            } else {
                zombie_char = 'z';
            }
            attron(COLOR_PAIR(COLOR_ZOMBIE) | A_BOLD);
            mvaddch(zy, zx, zombie_char);
            attroff(COLOR_PAIR(COLOR_ZOMBIE) | A_BOLD);
        } else if (game->zombies[i].death_anim > 0) {
            // Death animation
            const char death_frames[] = "X*+.";
            int frame = (EXPLOSION_DURATION - game->zombies[i].death_anim) / 2;
            if (frame >= 4) frame = 3;
            attron(COLOR_PAIR(COLOR_EXPLOSION) | A_BOLD);
            mvaddch(zy, zx, death_frames[frame]);
            attroff(COLOR_PAIR(COLOR_EXPLOSION) | A_BOLD);
        }
    }
    
    // Draw bullets with trail effect
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (game->bullets[i].active) {
            char ch;
            switch (game->bullets[i].dir) {
                case DIR_UP:    ch = '^'; break;
                case DIR_DOWN:  ch = 'v'; break;
                case DIR_LEFT:  ch = '<'; break;
                case DIR_RIGHT: ch = '>'; break;
                default:        ch = '*'; break;
            }
            int bx = game->bullets[i].x + sx;
            int by = game->bullets[i].y + sy;
            attron(COLOR_PAIR(COLOR_BULLET) | A_BOLD);
            mvaddch(by, bx, ch);
            attroff(COLOR_PAIR(COLOR_BULLET) | A_BOLD);
        }
    }
    
    // Draw player with direction indicator and invincibility flash
    int px = game->player.x + sx;
    int py = game->player.y + sy;
    
    if (game->player.invincible_ticks == 0 || game->tick % 2 == 0) {
        char player_char = get_player_char(game->last_dir);
        attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
        mvaddch(py, px, player_char);
        attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
    }
    
    // Status messages (centered)
    int msg_y = game->max_y / 2;
    int msg_x = game->max_x / 2;
    
    if (game->level_transition > 0 && !game->wave_cleared && !game->game_over) {
        const char *level_msg = "=== LEVEL %d ===";
        attron(COLOR_PAIR(COLOR_TITLE) | A_BOLD | A_BLINK);
        mvprintw(msg_y, msg_x - 8, level_msg, game->level);
        attroff(COLOR_PAIR(COLOR_TITLE) | A_BOLD | A_BLINK);
    }
    
    if (game->wave_cleared) {
        attron(COLOR_PAIR(COLOR_WAVE_CLEAR) | A_BOLD);
        mvprintw(msg_y - 1, msg_x - 12, "+---------------------------+");
        mvprintw(msg_y,     msg_x - 12, "|   WAVE CLEARED! +100 pts  |");
        mvprintw(msg_y + 1, msg_x - 12, "|  Press 'N' for next wave  |");
        mvprintw(msg_y + 2, msg_x - 12, "+---------------------------+");
        attroff(COLOR_PAIR(COLOR_WAVE_CLEAR) | A_BOLD);
    }
    
    if (game->game_over) {
        attron(COLOR_PAIR(COLOR_GAME_OVER) | A_BOLD);
        mvprintw(msg_y - 2, msg_x - 14, "+-----------------------------+");
        mvprintw(msg_y - 1, msg_x - 14, "|        GAME OVER!           |");
        mvprintw(msg_y,     msg_x - 14, "|     Final Score: %-5d      |", game->score);
        // Show NEW HIGH SCORE only if score equals the updated high_score
        // (meaning this score beat the previous record)
        if (game->score == game->high_score && game->score > 0) {
            mvprintw(msg_y + 1, msg_x - 14, "|      NEW HIGH SCORE!        |");
        } else if (game->high_score > 0) {
            mvprintw(msg_y + 1, msg_x - 14, "|     High Score: %-5d       |", game->high_score);
        } else {
            mvprintw(msg_y + 1, msg_x - 14, "|                             |");
        }
        mvprintw(msg_y + 2, msg_x - 14, "|    Press 'R' to restart     |");
        mvprintw(msg_y + 3, msg_x - 14, "+-----------------------------+");
        attroff(COLOR_PAIR(COLOR_GAME_OVER) | A_BOLD);
    }
    
    if (game->paused && !game->game_over && !game->wave_cleared) {
        attron(COLOR_PAIR(COLOR_HUD) | A_BOLD | A_REVERSE);
        mvprintw(msg_y, msg_x - 10, "    === PAUSED ===    ");
        mvprintw(msg_y + 1, msg_x - 10, " Press 'P' to resume  ");
        attroff(COLOR_PAIR(COLOR_HUD) | A_BOLD | A_REVERSE);
    }
}

void zombie_game_next_level(ZombieGame *game) {
    if (!game->wave_cleared) return;
    game->level++;
    if (game->zombie_move_period > 1) game->zombie_move_period--;
    spawn_zombies(game);
    // Reset player to center of game area
    game->player.x = game->game_area_x + game->game_area_w / 2;
    game->player.y = game->game_area_y + game->game_area_h / 2;
    for (int i = 0; i < MAX_BULLETS; ++i) game->bullets[i].active = 0;
    for (int i = 0; i < MAX_PARTICLES; ++i) game->particles[i].type = PARTICLE_NONE;
    game->wave_cleared = 0;
    game->level_transition = LEVEL_TRANSITION_DURATION;
}
