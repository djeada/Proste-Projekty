#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "version_control.h"

void print_help(void) {
    printf("\nSimple Version Control System\n");
    printf("==============================\n\n");
    printf("Commands:\n");
    printf("  init <path>      - Initialize repository at path\n");
    printf("  status           - Show repository status\n");
    printf("  commit <msg>     - Create a new commit with message\n");
    printf("  log              - Show commit history\n");
    printf("  checkout <id>    - Restore files from commit\n");
    printf("  diff <id1> <id2> - Show differences between commits\n");
    printf("  help             - Show this help\n");
    printf("  quit             - Exit program\n\n");
}

int main(int argc, char *argv[]) {
    Repository repo;
    char input[512];
    char command[64];
    char arg1[256];
    char arg2[256];
    int running = 1;

    memset(&repo, 0, sizeof(repo));

    printf("Simple Version Control System\n");
    printf("Type 'help' for available commands.\n");

    // Initialize from command line if path provided
    if (argc > 1) {
        if (repo_init(&repo, argv[1])) {
            printf("Initialized repository at: %s\n", argv[1]);
        } else {
            printf("Failed to initialize repository.\n");
        }
    }

    while (running) {
        printf("\nvcs> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            continue;
        }

        // Parse command and arguments
        arg1[0] = '\0';
        arg2[0] = '\0';
        int parsed = sscanf(input, "%63s %255[^\n]", command, arg1);

        if (strcmp(command, "help") == 0 || strcmp(command, "h") == 0) {
            print_help();
        }
        else if (strcmp(command, "quit") == 0 || strcmp(command, "q") == 0) {
            running = 0;
        }
        else if (strcmp(command, "init") == 0) {
            if (parsed < 2 || arg1[0] == '\0') {
                printf("Usage: init <path>\n");
            } else {
                if (repo_init(&repo, arg1)) {
                    printf("Initialized repository at: %s\n", arg1);
                } else {
                    printf("Failed to initialize repository.\n");
                }
            }
        }
        else if (strcmp(command, "status") == 0) {
            repo_status(&repo);
        }
        else if (strcmp(command, "commit") == 0) {
            if (!repo_is_initialized(&repo)) {
                printf("Repository not initialized. Use 'init <path>' first.\n");
            } else if (parsed < 2 || arg1[0] == '\0') {
                printf("Usage: commit <message>\n");
            } else {
                int id = repo_commit(&repo, arg1);
                if (id > 0) {
                    printf("Created commit #%d\n", id);
                } else {
                    printf("Failed to create commit.\n");
                }
            }
        }
        else if (strcmp(command, "log") == 0) {
            repo_log(&repo);
        }
        else if (strcmp(command, "checkout") == 0) {
            if (!repo_is_initialized(&repo)) {
                printf("Repository not initialized.\n");
            } else if (parsed < 2) {
                printf("Usage: checkout <commit_id>\n");
            } else {
                int id = atoi(arg1);
                if (!repo_checkout(&repo, id)) {
                    printf("Failed to checkout commit #%d.\n", id);
                }
            }
        }
        else if (strcmp(command, "diff") == 0) {
            if (!repo_is_initialized(&repo)) {
                printf("Repository not initialized.\n");
            } else {
                int id1, id2;
                if (sscanf(arg1, "%d %d", &id1, &id2) == 2) {
                    if (!repo_diff(&repo, id1, id2)) {
                        printf("Failed to diff commits.\n");
                    }
                } else {
                    printf("Usage: diff <commit_id1> <commit_id2>\n");
                }
            }
        }
        else {
            printf("Unknown command: %s. Type 'help' for available commands.\n", command);
        }
    }

    repo_free(&repo);
    printf("Goodbye!\n");
    return 0;
}
