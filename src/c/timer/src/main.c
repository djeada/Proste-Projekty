/* Terminal stopwatch and countdown. Keys: s start/stop, l lap, r reset, c countdown, q quit. */
#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "timer.h"

#define FRAME_MS 20
#define SHOWN_LAPS 8
#define LAPS_FILE "laps.txt"
#define MAX_TYPED_DIGITS 4
#define KEY_CTRL_C 3
#define KEY_ESC 27
#define KEY_BACKSPACE 127

typedef struct {
    Timer timer;
    int quit;
    int setting;       /* 1 while the countdown length is being typed */
    int typed;         /* typed digits, read as mmss */
    int typed_digits;
    int was_finished;
    char message[80];
} App;

static int64_t now_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static int enable_raw_mode(struct termios *saved)
{
    struct termios raw;

    if (!isatty(STDIN_FILENO) || tcgetattr(STDIN_FILENO, saved) != 0) {
        return 0;
    }
    raw = *saved;
    raw.c_lflag &= ~(ICANON | ECHO | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    return tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0;
}

static int save_laps(const Timer *t)
{
    FILE *file = fopen(LAPS_FILE, "w");
    char line[64];
    size_t i;

    if (!file) {
        return 0;
    }
    for (i = 0; i < t->lap_count; i++) {
        timer_format_lap(&t->laps[i], i + 1, line, sizeof line);
        fprintf(file, "%s\n", line);
    }
    return fclose(file) == 0;
}

static void handle_setting_key(App *app, int key)
{
    if (isdigit(key) && app->typed_digits < MAX_TYPED_DIGITS) {
        app->typed = app->typed * 10 + (key - '0');
        app->typed_digits++;
    } else if (key == KEY_BACKSPACE || key == '\b') {
        app->typed /= 10;
        app->typed_digits = app->typed_digits > 0 ? app->typed_digits - 1 : 0;
    } else if (key == KEY_ESC) {
        app->setting = 0;
    } else if (key == '\n' || key == '\r') {
        int minutes = app->typed / 100;
        int seconds = app->typed % 100;

        if (seconds > 59) {
            snprintf(app->message, sizeof app->message, "Seconds must be 0 to 59.");
            return;
        }
        timer_init(&app->timer, countdown_ms(minutes, seconds));
        app->setting = 0;
    }
}

static void handle_key(App *app, int key, int64_t now)
{
    if (key == KEY_CTRL_C) {
        app->quit = 1;
        return;
    }
    if (app->setting) {
        handle_setting_key(app, key);
        return;
    }
    app->message[0] = '\0';
    switch (tolower(key)) {
    case 's':
        if (app->timer.running) {
            timer_stop(&app->timer, now);
        } else {
            timer_start(&app->timer, now);
        }
        break;
    case 'l':
        if (timer_lap(&app->timer, now)) {
            if (save_laps(&app->timer)) {
                snprintf(app->message, sizeof app->message, "Lap saved to " LAPS_FILE ".");
            } else {
                snprintf(app->message, sizeof app->message, "Could not write " LAPS_FILE ".");
            }
        }
        break;
    case 'r':
        timer_reset(&app->timer);
        break;
    case 'c':
        app->setting = 1;
        app->typed = 0;
        app->typed_digits = 0;
        break;
    case 'q':
        app->quit = 1;
        break;
    }
}

static void draw(const App *app, int64_t now)
{
    const Timer *t = &app->timer;
    char text[32];
    char line[96];
    size_t i;
    size_t first = t->lap_count > SHOWN_LAPS ? t->lap_count - SHOWN_LAPS : 0;
    const char *state = t->running ? "running" : "stopped";

    if (timer_finished(t, now)) {
        state = "time is up";
    } else if (!t->running && timer_elapsed(t, now) == 0) {
        state = "ready";
    }
    timer_format(timer_display(t, now), text, sizeof text);

    printf("\x1b[H");
    printf("%s (%s)\x1b[K\r\n\r\n", t->limit_ms > 0 ? "Countdown" : "Stopwatch", state);
    printf("    \x1b[1m%s\x1b[0m\x1b[K\r\n\r\n", text);
    if (app->setting) {
        printf("Countdown mmss: %02d:%02d  (digits, Backspace, Enter sets, Enter on 0 gives a stopwatch, Esc cancels)\x1b[K\r\n",
               app->typed / 100, app->typed % 100);
    } else {
        printf("Keys: s start/stop  l lap  r reset  c countdown  q quit\x1b[K\r\n");
    }
    printf("%s\x1b[K\r\n\r\n", app->message);
    printf("Laps:\x1b[K\r\n");
    if (t->lap_count == 0) {
        printf("  no laps yet\x1b[K\r\n");
    }
    for (i = first; i < t->lap_count; i++) {
        timer_format_lap(&t->laps[i], i + 1, line, sizeof line);
        printf("  %s\x1b[K\r\n", line);
    }
    printf("\x1b[J");
    fflush(stdout);
}

static void wait_for_keys(App *app, int64_t now)
{
    struct timeval wait = {0, FRAME_MS * 1000};
    fd_set input;
    unsigned char keys[32];
    ssize_t count, i;

    FD_ZERO(&input);
    FD_SET(STDIN_FILENO, &input);
    if (select(STDIN_FILENO + 1, &input, NULL, NULL, &wait) <= 0) {
        return;
    }
    count = read(STDIN_FILENO, keys, sizeof keys);
    for (i = 0; i < count; i++) {
        handle_key(app, keys[i], now);
    }
}

int main(void)
{
    App app;
    struct termios saved;

    memset(&app, 0, sizeof app);
    timer_init(&app.timer, 0);
    if (!enable_raw_mode(&saved)) {
        fprintf(stderr, "timer needs an interactive terminal.\n");
        return 1;
    }
    printf("\x1b[2J\x1b[?25l");
    while (!app.quit) {
        int64_t now = now_ms();
        int finished;

        timer_tick(&app.timer, now);
        finished = timer_finished(&app.timer, now);
        if (finished && !app.was_finished) {
            printf("\a");
            snprintf(app.message, sizeof app.message, "Time is up!");
        }
        app.was_finished = finished;
        draw(&app, now);
        wait_for_keys(&app, now_ms());
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &saved);
    printf("\x1b[?25h\x1b[2J\x1b[H");
    return 0;
}
