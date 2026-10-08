/* Terminal interface: reads one expression per line and prints its value until "quit". */
#include "calculator.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    char line[1024];
    char error[128];
    double result;

    printf("Calculator. Type an expression, or 'quit' to exit.\n");
    for (;;) {
        printf("> ");
        fflush(stdout);
        if (fgets(line, sizeof line, stdin) == NULL) {
            break;
        }
        line[strcspn(line, "\r\n")] = '\0';
        if (strcmp(line, "quit") == 0) {
            break;
        }
        if (line[0] == '\0') {
            continue;
        }
        if (calc_evaluate(line, &result, error, sizeof error) == 0) {
            printf("%.10g\n", result);
        } else {
            printf("Error: %s\n", error);
        }
    }
    return 0;
}
