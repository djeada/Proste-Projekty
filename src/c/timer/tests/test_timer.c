#include "../src/timer.h"
#include <assert.h>
#include <signal.h>
#include <stdio.h>

void test_timer_loop_runs() {
    volatile sig_atomic_t running = 1;
    // This is a basic test to check the function can be called; in real tests, you would mock sleep/printf
    // Here, we just call and immediately stop
    running = 0;
    timer_loop(&running);
}

int main() {
    test_timer_loop_runs();
    printf("Timer logic tests passed.\n");
    return 0;
}
