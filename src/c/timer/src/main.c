#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t keep_running = 1;

void handle_signal(int signal) {
    // Signal handler to gracefully exit the loop
    keep_running = 0;
}

int main() {
    signal(SIGINT, handle_signal);  // Register the signal handler

    int secs = 0;
    int mins = 0;
    int hours = 0;

    while (keep_running) {
        secs++;
        mins = secs / 60;
        hours = mins / 60;

        // Format and display the time in HH:MM:SS format, updating on the same line
        printf("\r%02d:%02d:%02d", hours % 24, mins % 60, secs % 60);
        fflush(stdout);  // Flush the output buffer

        sleep(1);  // Wait for one second
    }

    printf("\nTimer stopped.\n");
    return 0;
}
