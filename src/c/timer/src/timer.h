#ifndef TIMER_H
#define TIMER_H

#include <signal.h>

void timer_loop(volatile sig_atomic_t *keep_running);

#endif // TIMER_H
