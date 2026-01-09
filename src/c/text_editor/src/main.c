#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_editor.h"

void print_help(void) {
    printf("\nCommands:\n");
    printf("  h        - Show this help\n");
    printf("  p        - Print all lines\n");
    printf("  a <text> - Append line\n");
    printf("  i <n> <text> - Insert line at position n\n");
    printf("  d <n>    - Delete line at position n\n");
    printf("  l <file> - Load file\n");
    printf("  s [file] - Save file\n");
    printf("  n        - New (clear buffer)\n");
    printf("  q        - Quit\n\n");
}

int main(int argc, char *argv[]) {
    TextBuffer buf;
    char command[MAX_LINE_LENGTH];
    char arg[MAX_LINE_LENGTH];
    int running = 1;

    buffer_init(&buf);

    printf("Simple Text Editor\n");
    printf("Type 'h' for help.\n");

    // Load file if provided as argument
    if (argc > 1) {
        if (buffer_load(&buf, argv[1])) {
            printf("Loaded: %s (%d lines)\n", argv[1], buf.num_lines);
        } else {
            printf("Could not load: %s\n", argv[1]);
        }
    }

    while (running) {
        printf("> ");
        if (!fgets(command, sizeof(command), stdin)) {
            break;
        }

        // Remove trailing newline
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        if (command[0] == '\0') {
            continue;
        }

        char cmd = command[0];
        char *rest = command + 1;
        while (*rest == ' ') rest++;

        switch (cmd) {
            case 'h':
                print_help();
                break;

            case 'p':
                if (buf.num_lines == 0) {
                    printf("(empty)\n");
                } else {
                    buffer_print(&buf);
                }
                break;

            case 'a':
                if (buffer_append_line(&buf, rest)) {
                    printf("Line added.\n");
                } else {
                    printf("Failed to add line.\n");
                }
                break;

            case 'i': {
                int pos;
                if (sscanf(rest, "%d %[^\n]", &pos, arg) == 2) {
                    if (buffer_insert_line(&buf, pos - 1, arg)) {
                        printf("Line inserted at %d.\n", pos);
                    } else {
                        printf("Failed to insert line.\n");
                    }
                } else {
                    printf("Usage: i <line_number> <text>\n");
                }
                break;
            }

            case 'd': {
                int pos;
                if (sscanf(rest, "%d", &pos) == 1) {
                    if (buffer_delete_line(&buf, pos - 1)) {
                        printf("Line %d deleted.\n", pos);
                    } else {
                        printf("Failed to delete line.\n");
                    }
                } else {
                    printf("Usage: d <line_number>\n");
                }
                break;
            }

            case 'l':
                if (rest[0] != '\0') {
                    if (buffer_load(&buf, rest)) {
                        printf("Loaded: %s (%d lines)\n", rest, buf.num_lines);
                    } else {
                        printf("Failed to load: %s\n", rest);
                    }
                } else {
                    printf("Usage: l <filename>\n");
                }
                break;

            case 's':
                if (rest[0] != '\0') {
                    if (buffer_save(&buf, rest)) {
                        printf("Saved: %s\n", rest);
                    } else {
                        printf("Failed to save: %s\n", rest);
                    }
                } else if (buf.filename[0] != '\0') {
                    if (buffer_save(&buf, NULL)) {
                        printf("Saved: %s\n", buf.filename);
                    } else {
                        printf("Failed to save.\n");
                    }
                } else {
                    printf("No filename. Usage: s <filename>\n");
                }
                break;

            case 'n':
                buffer_free(&buf);
                buffer_init(&buf);
                printf("Buffer cleared.\n");
                break;

            case 'q':
                if (buf.modified) {
                    printf("Unsaved changes. Save before quitting? (y/n): ");
                    if (fgets(arg, sizeof(arg), stdin)) {
                        if (arg[0] == 'y' || arg[0] == 'Y') {
                            if (buf.filename[0] != '\0') {
                                buffer_save(&buf, NULL);
                            } else {
                                printf("Enter filename: ");
                                if (fgets(arg, sizeof(arg), stdin)) {
                                    arg[strcspn(arg, "\n")] = '\0';
                                    buffer_save(&buf, arg);
                                }
                            }
                        }
                    }
                }
                running = 0;
                break;

            default:
                printf("Unknown command. Type 'h' for help.\n");
        }
    }

    buffer_free(&buf);
    printf("Goodbye!\n");
    return 0;
}
