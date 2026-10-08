#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "converter.h"

static int close_to(double a, double b) { return fabs(a - b) < 1e-9; }

int main(void) {
    assert(close_to(celsius_to_fahrenheit(0), 32));
    assert(close_to(celsius_to_fahrenheit(100), 212));
    assert(close_to(fahrenheit_to_celsius(-40), -40));

    double result;
    char unit;
    assert(convert(37, 'c', &result, &unit) && unit == 'F' && close_to(result, 98.6));
    assert(convert(212, 'F', &result, &unit) && unit == 'C' && close_to(result, 100));
    assert(!convert(10, 'K', &result, &unit));

    puts("All tests passed.");
    return 0;
}
