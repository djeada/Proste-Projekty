#include "timer.h"
#include <signal.h>

volatile sig_atomic_t keep_running = 1;

void handle_signal(int signal) {
    keep_running = 0;
}

int main() {
    signal(SIGINT, handle_signal);
    timer_loop(&keep_running);
    return 0;
}
