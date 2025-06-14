#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "calculator.h"

int main() {
    float num1, num2, result;
    char operator;
    bool running = true;
    int error;

    while (running) {
        printf("Enter an expression (e.g., 3.5 * 2) or 'q' to quit: ");
        if (scanf("%f %c %f", &num1, &operator, &num2) != 3) {
            char check;
            if(scanf(" %c", &check) && check == 'q') {
                running = false;
                continue;
            }
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        result = calculate(operator, num1, num2, &error);
        if (error == 1) {
            printf("Error: Division by zero.\n");
            continue;
        } else if (error == 2) {
            printf("Invalid operator.\n");
            continue;
        }

        printf("Result: %.2f\n", result);
    }

    printf("Calculator exited.\n");
    return 0;
}
