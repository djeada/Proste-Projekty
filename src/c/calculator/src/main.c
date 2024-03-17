#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    float num1, num2, result;
    char operator;
    bool running = true;

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

        switch (operator) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    printf("Error: Division by zero.\n");
                    continue;
                }
                result = num1 / num2;
                break;
            default:
                printf("Invalid operator.\n");
                continue;
        }

        printf("Result: %.2f\n", result);
    }

    printf("Calculator exited.\n");
    return 0;
}
