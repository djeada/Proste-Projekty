# Stopwatch and Countdown (C)

A terminal stopwatch with laps and a countdown that rings the terminal bell when it reaches zero. It is drawn with plain ANSI escape codes, redraws the time in place every 20 ms, and never blocks waiting for a key. Laps are saved to `laps.txt` as you record them.

The same program is also written in [Python](../../python/timer) and [JavaScript](../../vanilla_js/timer). They have the same features and rules.

![Screenshot](screenshot.png)

## Features

- Stopwatch with milliseconds: `mm:ss.mmm`, or `h:mm:ss.mmm` after an hour.
- Start, stop (pause), resume, and reset.
- Laps, each with its split time (since the previous lap) and total time.
- Countdown: type the length as digits, it stops by itself at zero and rings the bell.
- Laps are written to `laps.txt` in the current directory.
- Timing uses the monotonic clock, so changes to the system time do not affect it.

## How to use

| Key | Action |
|---|---|
| `s` | Start, or stop (pause) a running timer |
| `l` | Record a lap (only while running) |
| `r` | Reset the time and the laps |
| `c` | Set a countdown (see below) |
| `q` | Quit (Ctrl+C also works) |

Setting a countdown: press `c`, then type the digits of the length as `mmss`. `130` means 1 minute 30 seconds, `5` means 5 seconds. `Backspace` deletes a digit, `Enter` confirms, `Esc` cancels. Confirming `0` gives back a stopwatch. A countdown starts with `s`. When it reaches zero it stops, the bell rings, and "Time is up!" is shown. Press `r` to start again.

Example session:

```
s          start
l          lap 1
l          lap 2
s          pause
r          reset
c 130 Enter   countdown of 1 minute 30 seconds
s          start (the bell rings at zero)
```

## How it works

**Time values.** Every time is an integer number of milliseconds. The logic never reads a clock itself. The caller passes in `now`. This makes the rules testable with fixed numbers.

**The stopwatch state.** `timer.h` defines `Timer`:

- `accumulated_ms`: time from all finished running periods.
- `started_at_ms`: the clock value when the current running period began.
- `running`: 1 while the timer runs.

The elapsed time is `accumulated_ms + (now - started_at_ms)` while running, and just `accumulated_ms` while stopped. `timer_start` records `started_at_ms`. `timer_stop` adds the current period to `accumulated_ms`. Starting twice or stopping twice does nothing. Pausing and resuming therefore keep the time correctly.

**Laps.** `timer_lap` stores the total time now, and the split is the total minus the total of the previous lap. Laps are kept in a fixed array of 100 entries, so no memory is allocated. `main.c` rewrites `laps.txt` after each lap.

**Countdown.** A countdown is a `Timer` with `limit_ms` set. `timer_display` returns `limit - elapsed` instead of `elapsed`. The elapsed time is capped at the limit, so the display never goes below zero. `timer_tick` stops the timer once it reaches the limit. A finished countdown cannot be started again until it is reset.

**Why a monotonic clock.** A stopwatch measures how long something took. The wall clock (the time of day) is set by the operating system, and it can jump backwards or forwards when the network time sync corrects it, when the time zone or daylight saving time changes, or when someone changes the clock. A stopwatch that reads the wall clock could show a negative or a huge elapsed time. The monotonic clock (`CLOCK_MONOTONIC`) only counts forward from an arbitrary starting point, so differences between two readings are always correct. The program only uses differences.

**The interface loop.** `main.c` puts the terminal into raw mode: no line buffering, no echo, and Ctrl+C is read as a key. Each frame it:

1. reads the monotonic time and calls `timer_tick`,
2. rings the bell if the countdown has just finished,
3. redraws the screen (`ESC [H` moves to the top, then each line is written with `ESC [K` to clear the rest of it),
4. waits up to 20 ms with `select` for keys, so keys are handled as soon as they arrive.

Terminal settings are restored when the program quits.

## Project layout

```
CMakeLists.txt       build rules for the logic library, the program and the tests
README.md            this file
screenshot.png       a running stopwatch with three laps
src/timer.h          the declarations of the timer logic
src/timer.c          the timer logic: start, stop, lap, countdown, formatting
src/main.c           the terminal interface: raw keys, drawing, laps.txt
tests/test_timer.c   tests of the logic with fixed times
```

## Requirements

- A C compiler (GCC or Clang) and CMake 3.10 or newer
- Linux or macOS (the program uses POSIX terminal functions: `termios`, `select`, `clock_gettime`)

## Run

```sh
cmake -S . -B build
cmake --build build
./build/timer
```

Run it in a real terminal window. The program refuses to start when its input is not a terminal.

## Test

```sh
cmake -S . -B build && cmake --build build && cd build && ctest --output-on-failure
```

The tests call the logic with fixed numbers for `now`, so they do not wait or need a terminal.

## Comparison with the other versions

- [Python version](../../python/timer): tkinter window
- [JavaScript version](../../vanilla_js/timer): browser page

| | C | Python | JavaScript |
|---|---|---|---|
| Interface | terminal (ANSI codes) | tkinter window | browser page |
| Lines of logic | 136 | 73 | 75 |
| Lines of interface | 199 | 105 | 102 |
| Tests | 9 | 11 | 9 |

C needs the most code for the same features. It has to store laps in a fixed array, write the terminal mode itself, and convert each line to text with `snprintf` into a buffer. Python and JavaScript grow their lists on demand and print with a format string, so the logic is shorter. In C the `Timer` struct is passed by pointer and the caller owns its memory. In Python and JavaScript, the timer is an object that the garbage collector frees. In C the interface reads keys without blocking, while tkinter and the browser call a function every few milliseconds with timers.

## Ideas for extensions

- Show the time in large letters with a figlet-style font.
- Save laps as CSV, so spreadsheets can open them.
- Add a sound file played by the bell instead of the terminal bell.
- Let the countdown repeat (pomodoro: 25 minutes work, 5 minutes rest).
- Accept the countdown length on the command line: `./build/timer 5:00`.
