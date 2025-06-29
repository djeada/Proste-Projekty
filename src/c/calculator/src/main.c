#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "repl.h"

int main(int argc, char *argv[]) {
    if (argc > 1 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        printf("C Calculator - Interactive expression calculator\n");
        printf("Supports: +, -, *, /, ^, parentheses, unary operators\n");
        printf("Examples: 3.5 * 2, (2+3)*4, 2^3, -5+2\n");
        printf("Type 'exit' or 'quit' to exit\n");
        return 0;
    }

    calculator_repl();
    return 0;
}
