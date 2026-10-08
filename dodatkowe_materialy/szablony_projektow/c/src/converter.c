#include "converter.h"

#include <ctype.h>

double celsius_to_fahrenheit(double celsius) { return celsius * 9 / 5 + 32; }

double fahrenheit_to_celsius(double fahrenheit) { return (fahrenheit - 32) * 5 / 9; }

int convert(double value, char unit, double *result, char *result_unit) {
    switch (toupper((unsigned char)unit)) {
        case 'C':
            *result = celsius_to_fahrenheit(value);
            *result_unit = 'F';
            return 1;
        case 'F':
            *result = fahrenheit_to_celsius(value);
            *result_unit = 'C';
            return 1;
        default:
            return 0;
    }
}
