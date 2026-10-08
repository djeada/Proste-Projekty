/* Declarations of the expression logic: no input or output here. */
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stddef.h>

/* Evaluates an expression such as "2 + 3 * (4 - 1)".
   On success stores the value in *result and returns 0.
   On error writes a message into error and returns -1. */
int calc_evaluate(const char *text, double *result, char *error, size_t error_size);

#endif
