#include "repl.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 256

void calculator_repl(void) {
    char input[INPUT_SIZE];
    printf("C Calculator\n");
    printf("Enter mathematical expressions. Examples:\n");
    printf("  3.5 * 2\n");
    printf("  (2+3)*4\n");
    printf("  2^3\n");
    printf("  -5+2\n");
    printf("Type 'exit' or 'quit' to exit.\n\n");
    
    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        // Remove newline if present
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) continue; // Skip empty lines
        
        int error = 0;
        double result = parse_and_eval(input, &error);
        if (error == 0) {
            printf("= %g\n", result);
        } else {
            const char* error_msg;
            switch(error) {
                case 1: error_msg = "Syntax error or unexpected end of expression"; break;
                case 2: error_msg = "Division by zero"; break;
                case 3: error_msg = "Mismatched parentheses"; break;
                case 4: error_msg = "Invalid number or unexpected character"; break;
                default: error_msg = "Unknown error"; break;
            }
            printf("Error: %s\n", error_msg);
        }
    }
    printf("Calculator exited.\n");
}
