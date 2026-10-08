#ifndef TERM_H
#define TERM_H

#include <stdint.h>

#define COLS 64
#define ROWS 22
#define W COLS
#define H (ROWS * 2)

extern uint32_t pixels[H][W];
extern char text[ROWS][COLS];
extern uint32_t ink[ROWS][COLS];

void start(int argc, char **argv, int delay_ms);
void show_pixels(const char *status);
void show_text(const char *status);
void seed(uint32_t s);
int rnd(int n);

#endif
