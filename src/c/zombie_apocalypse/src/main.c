#include "zombie_apocalypse.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>

#define MIN_TERM_WIDTH 60
#define MIN_TERM_HEIGHT 20
#define KEY_ESCAPE 27

int main(void) {
    // Initialize ncurses
    initscr();
    
    // Check terminal size
    int max_y;
    int max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    if (max_x < MIN_TERM_WIDTH || max_y < MIN_TERM_HEIGHT) {
        endwin();
        fprintf(stderr, "Terminal too small! Need at least %dx%d, got %dx%d\n",
                MIN_TERM_WIDTH, MIN_TERM_HEIGHT, max_x, max_y);
        return 1;
    }
    
    // Setup ncurses options
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(80); // Slightly faster refresh for smoother animations
    
    // Initialize colors
    zombie_game_init_colors();
    
    // Seed random number generator
    srand((unsigned int)time(NULL));
    
    // Initialize game
    ZombieGame game;
    memset(&game, 0, sizeof(ZombieGame));
    zombie_game_init(&game, max_x, max_y);
    
    // Main game loop
    int running = 1;
    while (running) {
        // Handle terminal resize
        int new_y;
        int new_x;
        getmaxyx(stdscr, new_y, new_x);
        if (new_x != game.max_x || new_y != game.max_y) {
            if (new_x >= MIN_TERM_WIDTH && new_y >= MIN_TERM_HEIGHT) {
                zombie_game_init(&game, new_x, new_y);
            }
        }
        
        // Clear and draw
        erase();
        zombie_game_draw(&game);
        
        // Get input
        int key = getch();
        
        // Quit on 'q' or ESC
        if (key == 'q' || key == KEY_ESCAPE) {
            running = 0;
            continue;
        }
        
        // Update game state
        zombie_game_update(&game, key);
        
        // Refresh display
        refresh();
    }
    
    // Cleanup
    endwin();
    
    // Print final stats
    printf("\n=== ZOMBIE APOCALYPSE ===\n");
    printf("Thanks for playing!\n");
    printf("Final Score: %d\n", game.score);
    printf("Waves Completed: %d\n", game.level - 1);
    if (game.high_score > 0) {
        printf("High Score: %d\n", game.high_score);
    }
    printf("\n");
    
    return 0;
}
