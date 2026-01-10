#include "game_2048.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

static struct termios original_termios;
static int termios_saved = 0;

static void restore_terminal(void) {
    if (termios_saved) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    }
}

static void setup_terminal(void) {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &original_termios);
    termios_saved = 1;
    atexit(restore_terminal);
    
    newt = original_termios;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

static int getch_with_arrows(G2048Move *move) {
    int ch = getchar();
    
    /* Check for escape sequence (arrow keys) */
    if (ch == 27) {  /* ESC */
        int next = getchar();
        if (next == '[') {
            int arrow = getchar();
            switch (arrow) {
                case 'A': *move = MOVE_UP;    return 1;  /* Up arrow */
                case 'B': *move = MOVE_DOWN;  return 1;  /* Down arrow */
                case 'C': *move = MOVE_RIGHT; return 1;  /* Right arrow */
                case 'D': *move = MOVE_LEFT;  return 1;  /* Left arrow */
                default: break;
            }
        }
        return 0;  /* Unknown escape sequence */
    }
    
    /* WASD keys */
    switch (ch) {
        case 'w': case 'W': *move = MOVE_UP;    return 1;
        case 's': case 'S': *move = MOVE_DOWN;  return 1;
        case 'a': case 'A': *move = MOVE_LEFT;  return 1;
        case 'd': case 'D': *move = MOVE_RIGHT; return 1;
        case 'q': case 'Q': return -1;  /* Quit */
        case 'r': case 'R': return -2;  /* Restart */
        default: break;
    }
    
    return 0;  /* Unknown key */
}

int main(void) {
    g2048_seed_random((unsigned)time(NULL));
    G2048 g;
    g2048_init(&g);
    
    setup_terminal();
    
    while (1) {
        g2048_draw_text(&g, stdout);
        
        G2048Move move;
        int result = getch_with_arrows(&move);
        
        if (result == -1) {  /* Quit */
            break;
        }
        
        if (g.game_over) {
            if (result == -2) {  /* Restart */
                g2048_init(&g);
            }
            continue;
        }
        
        if (result == 1) {  /* Valid move */
            g2048_move(&g, move);
        }
    }
    
    /* Clear screen on exit */
    printf("\033[2J\033[H");
    printf("Thanks for playing 2048! Final score: %d\n", g.score);
    
    return 0;
}
