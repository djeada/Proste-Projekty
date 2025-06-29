#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"
#include "repl.h"

void print_usage() {
    printf("C Calculator - Usage:\n");
    printf("  calculator simple    - Use simple mode (num1 operator num2)\n");
    printf("  calculator advanced  - Use advanced mode (complex expressions)\n");
    printf("  calculator           - Use advanced mode (default)\n");
}

void simple_mode() {
    float num1, num2, result;
    char operator;
    int error;
    char input[10];

    printf("Simple Calculator Mode\n");
    printf("Enter expressions like: 3.5 * 2\n");
    printf("Type 'q' to quit\n\n");

    while (1) {
        printf("> ");
        if (scanf("%f %c %f", &num1, &operator, &num2) != 3) {
            // Check if user wants to quit
            if(scanf("%9s", input) && strcmp(input, "q") == 0) {
                break;
            }
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        result = calculate(operator, &error, num1, num2);
        if (error == 1) {
            printf("Error: Division by zero.\n");
        } else if (error == 2) {
            printf("Error: Invalid operator.\n");
        } else {
            printf("= %.2f\n", result);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "simple") == 0) {
            simple_mode();
        } else if (strcmp(argv[1], "advanced") == 0) {
            calculator_repl();
        } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            print_usage();
        } else {
            printf("Unknown option: %s\n", argv[1]);
            print_usage();
            return 1;
        }
    } else {
        // Default to advanced mode
        calculator_repl();
    }

    return 0;
}
