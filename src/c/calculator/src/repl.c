#include "repl.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 256

void calculator_repl(void) {
    char input[INPUT_SIZE];
    printf("Welcome to the advanced C calculator!\nType an expression or 'exit' to quit.\n");
    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        if (strncmp(input, "exit", 4) == 0) break;
        int error = 0;
        double result = parse_and_eval(input, &error);
        if (error == 0) {
            printf("= %g\n", result);
        } else {
            printf("Error: invalid expression (code %d)\n", error);
        }
    }
}
