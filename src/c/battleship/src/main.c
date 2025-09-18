#include "battleship.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

// Read a single character without waiting for Enter (POSIX)
int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main() {
    srand((unsigned)time(NULL));
    BattleGame game; battleship_init(&game, 80, 24);
    puts("Controls: WASD to move, r rotate, Enter place/fire, q quit demo");
    while (1) {
        battleship_draw_text(&game, stdout);
        int ch = getch();
        if (ch == EOF) break;
        battleship_update(&game, ch);
        if (game.phase == PHASE_QUIT) break;
        puts("");
    }
    return 0;
}
