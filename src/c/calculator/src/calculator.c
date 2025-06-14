#include "calculator.h"

float calculate(char operator, int* error, float num1, float num2) {
    *error = 0;
    switch (operator) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 == 0) {
                *error = 1;
                return 0.0f;
            }
            return num1 / num2;
        default:
            *error = 2;
            return 0.0f;
    }
}
