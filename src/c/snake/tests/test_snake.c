#include <assert.h>
#include <stdio.h>
#include <ncurses.h>

// Dummy test to check if ncurses initializes and ends without error
int main() {
    initscr();
    assert(stdscr != NULL);
    endwin();
    printf("Ncurses initialized and ended successfully.\n");
    return 0;
}
