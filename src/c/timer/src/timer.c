/* Stopwatch and countdown rules. */
#include "timer.h"

#include <stdio.h>

static int64_t raw_elapsed(const Timer *t, int64_t now)
{
    if (!t->running) {
        return t->accumulated_ms;
    }
    return t->accumulated_ms + (now - t->started_at_ms);
}

void timer_init(Timer *t, int64_t limit_ms)
{
    t->limit_ms = limit_ms;
    timer_reset(t);
}

void timer_reset(Timer *t)
{
    t->accumulated_ms = 0;
    t->started_at_ms = 0;
    t->running = 0;
    t->lap_count = 0;
}

void timer_start(Timer *t, int64_t now)
{
    if (t->running || timer_finished(t, now)) {
        return;
    }
    t->running = 1;
    t->started_at_ms = now;
}

void timer_stop(Timer *t, int64_t now)
{
    if (!t->running) {
        return;
    }
    t->accumulated_ms = raw_elapsed(t, now);
    t->running = 0;
}

int timer_lap(Timer *t, int64_t now)
{
    int64_t total;
    int64_t previous = 0;

    if (!t->running || t->lap_count >= TIMER_MAX_LAPS) {
        return 0;
    }
    total = timer_elapsed(t, now);
    if (t->lap_count > 0) {
        previous = t->laps[t->lap_count - 1].total_ms;
    }
    t->laps[t->lap_count].total_ms = total;
    t->laps[t->lap_count].split_ms = total - previous;
    t->lap_count++;
    return 1;
}

void timer_tick(Timer *t, int64_t now)
{
    if (t->running && t->limit_ms > 0 && raw_elapsed(t, now) >= t->limit_ms) {
        timer_stop(t, now);
    }
}

int64_t timer_elapsed(const Timer *t, int64_t now)
{
    int64_t elapsed = raw_elapsed(t, now);

    if (t->limit_ms > 0 && elapsed > t->limit_ms) {
        return t->limit_ms;
    }
    return elapsed;
}

int64_t timer_display(const Timer *t, int64_t now)
{
    if (t->limit_ms > 0) {
        return t->limit_ms - timer_elapsed(t, now);
    }
    return timer_elapsed(t, now);
}

int timer_finished(const Timer *t, int64_t now)
{
    return t->limit_ms > 0 && timer_elapsed(t, now) >= t->limit_ms;
}

int64_t countdown_ms(int minutes, int seconds)
{
    return ((int64_t)minutes * 60 + seconds) * 1000;
}

int timer_format(int64_t ms, char *buf, size_t size)
{
    int64_t hours = ms / 3600000;
    int minutes = (int)(ms / 60000 % 60);
    int seconds = (int)(ms / 1000 % 60);
    int millis = (int)(ms % 1000);

    if (hours > 0) {
        return snprintf(buf, size, "%lld:%02d:%02d.%03d", (long long)hours, minutes, seconds, millis);
    }
    return snprintf(buf, size, "%02d:%02d.%03d", minutes, seconds, millis);
}

int timer_format_lap(const Lap *lap, size_t number, char *buf, size_t size)
{
    char split[32];
    char total[32];

    timer_format(lap->split_ms, split, sizeof split);
    timer_format(lap->total_ms, total, sizeof total);
    return snprintf(buf, size, "Lap %zu: split %s  total %s", number, split, total);
}
