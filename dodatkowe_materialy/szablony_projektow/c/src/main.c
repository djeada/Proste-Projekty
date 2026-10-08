#include <stdio.h>

#include "converter.h"

int main(void) {
    double value, result;
    char unit, result_unit;

    printf("Temperature (e.g. 36.6 C or 98 F): ");
    if (scanf("%lf %c", &value, &unit) != 2 || !convert(value, unit, &result, &result_unit)) {
        fprintf(stderr, "Expected a number followed by C or F.\n");
        return 1;
    }
    printf("%.1f %c = %.1f %c\n", value, unit, result, result_unit);
    return 0;
}
