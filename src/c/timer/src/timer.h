/* Stopwatch and countdown rules. Time is given in milliseconds by the caller. */
#ifndef TIMER_H
#define TIMER_H

#include <stddef.h>
#include <stdint.h>

#define TIMER_MAX_LAPS 100

typedef struct {
    int64_t split_ms; /* time since the previous lap */
    int64_t total_ms; /* time since the start */
} Lap;

typedef struct {
    int64_t limit_ms;       /* countdown length, 0 for a stopwatch */
    int64_t accumulated_ms; /* time of the finished running periods */
    int64_t started_at_ms;  /* monotonic time when the current period began */
    int running;
    size_t lap_count;
    Lap laps[TIMER_MAX_LAPS];
} Timer;

void timer_init(Timer *t, int64_t limit_ms);
void timer_start(Timer *t, int64_t now);
void timer_stop(Timer *t, int64_t now);
int timer_lap(Timer *t, int64_t now); /* returns 1 if a lap was recorded */
void timer_reset(Timer *t);
/* Stops a countdown that has reached zero. Call it regularly. */
void timer_tick(Timer *t, int64_t now);

int64_t timer_elapsed(const Timer *t, int64_t now);
int64_t timer_display(const Timer *t, int64_t now); /* elapsed, or remaining in countdown mode */
int timer_finished(const Timer *t, int64_t now);

int64_t countdown_ms(int minutes, int seconds);
/* Writes "hh:mm:ss.mmm" with hours only when needed. Returns the number of characters. */
int timer_format(int64_t ms, char *buf, size_t size);
/* Writes one line of the laps file, for example "Lap 2: split 00:05.120  total 00:12.300". */
int timer_format_lap(const Lap *lap, size_t number, char *buf, size_t size);

#endif /* TIMER_H */
