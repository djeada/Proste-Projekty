/* Tests of the timer logic. Time is passed in directly, so no real clock is needed. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "timer.h"

static void test_format(void)
{
    char buf[32];

    timer_format(0, buf, sizeof buf);
    assert(strcmp(buf, "00:00.000") == 0);
    timer_format(65432, buf, sizeof buf);
    assert(strcmp(buf, "01:05.432") == 0);
    timer_format(3723004, buf, sizeof buf);
    assert(strcmp(buf, "1:02:03.004") == 0);
}

static void test_format_lap(void)
{
    Lap lap = {5120, 12300};
    char buf[64];

    timer_format_lap(&lap, 2, buf, sizeof buf);
    assert(strcmp(buf, "Lap 2: split 00:05.120  total 00:12.300") == 0);
}

static void test_stopwatch_pauses(void)
{
    Timer t;

    timer_init(&t, 0);
    assert(timer_elapsed(&t, 0) == 0);
    timer_start(&t, 1000);
    assert(timer_elapsed(&t, 1500) == 500);
    timer_stop(&t, 2000);
    assert(timer_elapsed(&t, 9000) == 1000);
    timer_start(&t, 9000);
    assert(timer_elapsed(&t, 9250) == 1250);
}

static void test_start_and_stop_are_idempotent(void)
{
    Timer t;

    timer_init(&t, 0);
    timer_stop(&t, 100);
    assert(timer_elapsed(&t, 200) == 0);
    timer_start(&t, 0);
    timer_start(&t, 500);
    assert(timer_elapsed(&t, 1000) == 1000);
}

static void test_laps_split_and_total(void)
{
    Timer t;

    timer_init(&t, 0);
    assert(timer_lap(&t, 0) == 0);
    timer_start(&t, 0);
    assert(timer_lap(&t, 1000) == 1);
    assert(timer_lap(&t, 1500) == 1);
    assert(t.laps[0].total_ms == 1000 && t.laps[0].split_ms == 1000);
    assert(t.laps[1].total_ms == 1500 && t.laps[1].split_ms == 500);
    timer_stop(&t, 2000);
    assert(timer_lap(&t, 3000) == 0);
    assert(t.lap_count == 2);
}

static void test_reset_clears_everything(void)
{
    Timer t;

    timer_init(&t, 0);
    timer_start(&t, 0);
    timer_lap(&t, 100);
    timer_stop(&t, 200);
    timer_reset(&t);
    assert(!t.running);
    assert(t.lap_count == 0);
    assert(timer_elapsed(&t, 500) == 0);
}

static void test_countdown_remaining_and_finish(void)
{
    Timer t;

    timer_init(&t, countdown_ms(0, 3));
    assert(countdown_ms(1, 30) == 90000);
    assert(timer_display(&t, 0) == 3000);
    timer_start(&t, 0);
    assert(timer_display(&t, 1000) == 2000);
    assert(!timer_finished(&t, 2999));
    assert(timer_finished(&t, 3000));
    assert(timer_display(&t, 5000) == 0);
    timer_tick(&t, 5000);
    assert(!t.running);
    assert(timer_elapsed(&t, 9000) == 3000);
    timer_start(&t, 9000);
    assert(!t.running);
}

static void test_countdown_tick_before_zero_keeps_running(void)
{
    Timer t;

    timer_init(&t, 2000);
    timer_start(&t, 0);
    timer_tick(&t, 1999);
    assert(t.running);
    timer_tick(&t, 2000);
    assert(!t.running);
}

static void test_countdown_reset_allows_restart(void)
{
    Timer t;

    timer_init(&t, 1000);
    timer_start(&t, 0);
    timer_tick(&t, 1000);
    timer_reset(&t);
    timer_start(&t, 2000);
    assert(t.running);
    assert(timer_display(&t, 2500) == 500);
}

int main(void)
{
    test_format();
    test_format_lap();
    test_stopwatch_pauses();
    test_start_and_stop_are_idempotent();
    test_laps_split_and_total();
    test_reset_clears_everything();
    test_countdown_remaining_and_finish();
    test_countdown_tick_before_zero_keeps_running();
    test_countdown_reset_allows_restart();
    printf("All 9 timer tests passed.\n");
    return 0;
}
