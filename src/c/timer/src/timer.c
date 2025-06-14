#include "timer.h"
#include <stdio.h>
#include <unistd.h>

void timer_loop(volatile sig_atomic_t *keep_running) {
    int secs = 0;
    int mins = 0;
    int hours = 0;

    while (*keep_running) {
        secs++;
        mins = secs / 60;
        hours = mins / 60;
        printf("\r%02d:%02d:%02d", hours % 24, mins % 60, secs % 60);
        fflush(stdout);
        sleep(1);
    }
    printf("\nTimer stopped.\n");
}
