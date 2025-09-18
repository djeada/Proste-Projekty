#include "game_2048.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

static int getch_nowait() {
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
    g2048_seed_random((unsigned)time(NULL));
    G2048 g; g2048_init(&g);
    while (1) {
        g2048_draw_text(&g, stdout);
        int ch = getch_nowait();
        if (ch == 'q') break;
        if (g.game_over && ch == 'r') { g2048_init(&g); continue; }
        if (ch == 'w') g2048_move(&g, MOVE_UP);
        else if (ch == 's') g2048_move(&g, MOVE_DOWN);
        else if (ch == 'a') g2048_move(&g, MOVE_LEFT);
        else if (ch == 'd') g2048_move(&g, MOVE_RIGHT);
    }
    return 0;
}
