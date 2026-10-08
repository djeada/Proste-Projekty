/* Command line interface of the version control tool. Works on the current directory. */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "version_control.h"

static const char *change_name(ChangeKind kind) {
    switch (kind) {
    case CHANGE_ADDED:
        return "added";
    case CHANGE_MODIFIED:
        return "modified";
    default:
        return "deleted";
    }
}

static void print_usage(void) {
    printf("Usage: version_control <command> [argument]\n\n");
    printf("Commands:\n");
    printf("  init               create a repository in this directory\n");
    printf("  commit \"message\"   save all files as a new commit\n");
    printf("  log                list commits, newest first\n");
    printf("  status             show files changed since the last commit\n");
    printf("  diff [N]           show changed lines compared with commit N (default: last)\n");
    printf("  checkout N         restore the files of commit N\n");
}

static int not_a_repository(void) {
    printf("Not a repository. Run 'version_control init' first.\n");
    return 1;
}

static int parse_number(const char *text, int *number) {
    char *end;
    long value = strtol(text, &end, 10);
    if (*text == '\0' || *end != '\0' || value < 1 || value > INT_MAX) {
        return 0;
    }
    *number = (int)value;
    return 1;
}

static int cmd_init(void) {
    if (repo_init(".") != 0) {
        printf("Could not create a repository (already initialized?).\n");
        return 1;
    }
    printf("Initialized empty repository in ./%s\n", VCS_DIR);
    return 0;
}

static int cmd_commit(const char *message) {
    int number;
    if (repo_commit_count(".") < 0) {
        return not_a_repository();
    }
    number = repo_commit(".", message);
    if (number < 0) {
        printf("Commit failed. The message must be one line and shorter than %d characters.\n",
               MESSAGE_SIZE - 1);
        return 1;
    }
    printf("Created commit #%d\n", number);
    return 0;
}

static int cmd_log(void) {
    int count = repo_commit_count(".");
    if (count < 0) {
        return not_a_repository();
    }
    if (count == 0) {
        printf("No commits yet.\n");
        return 0;
    }
    for (int number = count; number >= 1; number--) {
        CommitInfo info;
        char date[32];
        if (repo_info(".", number, &info) != 0) {
            printf("Cannot read commit #%d.\n", number);
            return 1;
        }
        format_time(info.time, date, sizeof(date));
        printf("#%-4d %s  %s\n", info.number, date, info.message);
    }
    return 0;
}

static int cmd_status(void) {
    Snapshot base = {NULL, 0};
    Snapshot current;
    int count = repo_commit_count(".");
    if (count < 0) {
        return not_a_repository();
    }
    if (count > 0 && repo_load_commit(".", count, &base) != 0) {
        printf("Cannot read commit #%d.\n", count);
        return 1;
    }
    if (snapshot_load_dir(".", &current) != 0) {
        printf("Cannot read the current directory.\n");
        return 1;
    }

    Change *changes = malloc((base.count + current.count + 1) * sizeof(Change));
    size_t n = snapshot_changes(&base, &current, changes);
    if (count == 0) {
        printf("No commits yet.\n");
    } else {
        printf("Changes since commit #%d:\n", count);
    }
    if (n == 0) {
        printf("  nothing changed\n");
    }
    for (size_t i = 0; i < n; i++) {
        printf("  %-10s %s\n", change_name(changes[i].kind), changes[i].name);
    }

    free(changes);
    snapshot_free(&base);
    snapshot_free(&current);
    return 0;
}

static int cmd_diff(int argc, char *argv[]) {
    Snapshot base = {NULL, 0};
    Snapshot current;
    int count = repo_commit_count(".");
    int number = count;

    if (count < 0) {
        return not_a_repository();
    }
    if (count == 0) {
        printf("No commits yet.\n");
        return 1;
    }
    if (argc == 3 && !parse_number(argv[2], &number)) {
        print_usage();
        return 1;
    }
    if (number > count) {
        printf("There is no commit #%d.\n", number);
        return 1;
    }
    if (repo_load_commit(".", number, &base) != 0 || snapshot_load_dir(".", &current) != 0) {
        printf("Cannot read commit #%d or the current directory.\n", number);
        return 1;
    }

    Change *changes = malloc((base.count + current.count + 1) * sizeof(Change));
    size_t n = snapshot_changes(&base, &current, changes);
    if (n == 0) {
        printf("No changes since commit #%d.\n", number);
    }
    for (size_t i = 0; i < n; i++) {
        const char *name = changes[i].name;
        const File *old_file = changes[i].kind == CHANGE_ADDED ? NULL : snapshot_find(&base, name);
        const File *new_file = changes[i].kind == CHANGE_DELETED ? NULL : snapshot_find(&current, name);
        DiffLine *lines;
        size_t line_count;

        printf("== %s (%s)\n", name, change_name(changes[i].kind));
        if (diff_file(old_file, new_file, &lines, &line_count) != 0) {
            printf("Out of memory.\n");
            break;
        }
        for (size_t k = 0; k < line_count; k++) {
            if (lines[k].tag != ' ') {
                printf("%c%.*s\n", lines[k].tag, (int)lines[k].length, (const char *)lines[k].text);
            }
        }
        free(lines);
    }

    free(changes);
    snapshot_free(&base);
    snapshot_free(&current);
    return 0;
}

static int cmd_checkout(const char *text) {
    int number;
    if (repo_commit_count(".") < 0) {
        return not_a_repository();
    }
    if (!parse_number(text, &number)) {
        printf("Commit number must be a positive integer.\n");
        return 1;
    }
    if (repo_checkout(".", number) != 0) {
        printf("Could not restore commit #%d.\n", number);
        return 1;
    }
    printf("Restored the files of commit #%d\n", number);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    const char *command = argv[1];
    if (strcmp(command, "init") == 0 && argc == 2) {
        return cmd_init();
    }
    if (strcmp(command, "commit") == 0 && argc == 3) {
        return cmd_commit(argv[2]);
    }
    if (strcmp(command, "log") == 0 && argc == 2) {
        return cmd_log();
    }
    if (strcmp(command, "status") == 0 && argc == 2) {
        return cmd_status();
    }
    if (strcmp(command, "diff") == 0 && (argc == 2 || argc == 3)) {
        return cmd_diff(argc, argv);
    }
    if (strcmp(command, "checkout") == 0 && argc == 3) {
        return cmd_checkout(argv[2]);
    }

    print_usage();
    return 1;
}
