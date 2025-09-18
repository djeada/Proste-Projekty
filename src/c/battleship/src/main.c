#include "battleship.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    BattleGame game; battleship_init(&game, 80, 24);
    puts("Controls: WASD to move, r rotate, Enter place/fire, q quit demo");
    while (1) {
        battleship_draw_text(&game, stdout);
        int ch = getchar();
        if (ch == EOF) break;
        battleship_update(&game, ch);
        puts("");
    }
    return 0;
}
