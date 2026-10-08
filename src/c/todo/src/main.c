/* Terminal interface: parses the command line and keeps the tasks in tasks.txt. */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "todo.h"

#define STORAGE_FILE "tasks.txt"
#define RED "\033[31m"
#define RESET "\033[0m"

typedef struct {
    const char *positional; /* the title for add, the id for edit, done, undone and rm */
    const char *title;
    const char *due;
    const char *priority;
    const char *category;
    const char *status;
} Args;

static void print_usage(FILE *out) {
    fputs("usage: todo [list] [--category NAME] [--status all|active|done]\n"
          "       todo add TITLE [--due YYYY-MM-DD] [--priority low|medium|high] [--category NAME]\n"
          "       todo edit ID [--title TITLE] [--due YYYY-MM-DD|none] [--priority P] "
          "[--category NAME|none]\n"
          "       todo done ID | todo undone ID | todo rm ID\n",
          out);
}

static bool parse_args(int argc, char **argv, Args *args) {
    for (int i = 2; i < argc; ++i) {
        const char *arg = argv[i];
        if (strncmp(arg, "--", 2) != 0) {
            if (args->positional != NULL) {
                return false;
            }
            args->positional = arg;
            continue;
        }
        if (i + 1 >= argc) {
            return false;
        }
        const char *value = argv[++i];
        if (strcmp(arg, "--title") == 0) {
            args->title = value;
        } else if (strcmp(arg, "--due") == 0) {
            args->due = value;
        } else if (strcmp(arg, "--priority") == 0) {
            Priority priority;
            if (!parse_priority(value, &priority)) {
                return false;
            }
            args->priority = value;
        } else if (strcmp(arg, "--category") == 0) {
            args->category = value;
        } else if (strcmp(arg, "--status") == 0) {
            args->status = value;
        } else {
            return false;
        }
    }
    return true;
}

static bool load_tasks(TaskList *list) {
    FILE *file = fopen(STORAGE_FILE, "r");
    if (file == NULL) {
        return errno == ENOENT;
    }
    char line[LINE_LEN];
    int number = 0;
    bool ok = true;
    while (ok && fgets(line, sizeof line, file) != NULL) {
        number++;
        if (strspn(line, "\r\n") == strlen(line)) {
            continue;
        }
        Task task;
        if (!task_from_line(line, &task) || list->count >= MAX_TASKS) {
            fprintf(stderr, "%s: line %d is not a valid task\n", STORAGE_FILE, number);
            ok = false;
        } else {
            list->tasks[list->count++] = task;
        }
    }
    fclose(file);
    return ok;
}

static bool save_tasks(const TaskList *list) {
    FILE *file = fopen(STORAGE_FILE, "w");
    if (file == NULL) {
        return false;
    }
    char line[LINE_LEN];
    bool ok = true;
    for (int i = 0; i < list->count && ok; ++i) {
        ok = task_to_line(&list->tasks[i], line, sizeof line) && fprintf(file, "%s\n", line) >= 0;
    }
    return fclose(file) == 0 && ok;
}

static void today_string(char *buf, size_t size) {
    time_t now = time(NULL);
    strftime(buf, size, "%Y-%m-%d", localtime(&now));
}

static bool parse_id(const char *text, int *id) {
    char *end = NULL;
    long value = strtol(text, &end, 10);
    if (text[0] == '\0' || *end != '\0' || value <= 0 || value > 1000000000L) {
        return false;
    }
    *id = (int)value;
    return true;
}

static Task *find_task(TaskList *list, const char *text) {
    int id = 0;
    if (text == NULL || !parse_id(text, &id)) {
        return NULL;
    }
    return list_find(list, id);
}

static bool set_text(char *dst, size_t size, const char *value) {
    if (strlen(value) >= size) {
        return false;
    }
    strcpy(dst, value);
    return true;
}

/* In edit commands the word "none" removes the due date or the category. */
static const char *none_means_empty(const char *value) {
    return strcmp(value, "none") == 0 ? "" : value;
}

static bool apply_fields(Task *task, const Args *args) {
    if (args->due != NULL && !set_text(task->due, sizeof task->due, none_means_empty(args->due))) {
        return false;
    }
    if (args->priority != NULL && !parse_priority(args->priority, &task->priority)) {
        return false;
    }
    return args->category == NULL ||
           set_text(task->category, sizeof task->category, none_means_empty(args->category));
}

static int report_save(bool saved) {
    if (!saved) {
        fprintf(stderr, "could not write %s\n", STORAGE_FILE);
        return 1;
    }
    return 0;
}

static int cmd_list(const TaskList *list, const Args *args) {
    StatusFilter status = STATUS_ALL;
    if (args->status != NULL) {
        if (strcmp(args->status, "active") == 0) {
            status = STATUS_ACTIVE;
        } else if (strcmp(args->status, "done") == 0) {
            status = STATUS_DONE;
        } else if (strcmp(args->status, "all") != 0) {
            print_usage(stderr);
            return 2;
        }
    }

    const Task *views[MAX_TASKS];
    size_t count = list_view(list, args->category, status, views);
    if (count == 0) {
        puts("No tasks.");
        return 0;
    }

    char today[DATE_LEN];
    today_string(today, sizeof today);
    printf("%-4s %-6s %-8s %-10s %-12s %s\n", "ID", "STATUS", "PRIORITY", "DUE", "CATEGORY",
           "TITLE");
    for (size_t i = 0; i < count; ++i) {
        const Task *task = views[i];
        bool overdue = task_is_overdue(task, today);
        const char *mark = task->done ? "[x]" : (overdue ? "[!]" : "[ ]");
        printf("%s%-4d %-6s %-8s %-10s %-12s %s%s\n", overdue ? RED : "", task->id, mark,
               priority_name(task->priority), task->due[0] ? task->due : "-",
               task->category[0] ? task->category : "-", task->title, overdue ? RESET : "");
    }
    return 0;
}

static int cmd_add(TaskList *list, const Args *args) {
    if (args->positional == NULL) {
        print_usage(stderr);
        return 2;
    }
    if (list->count >= MAX_TASKS) {
        fputs("the list is full\n", stderr);
        return 1;
    }
    Task task = {0};
    task.priority = PRIORITY_MEDIUM;
    if (!set_text(task.title, sizeof task.title, args->positional) || !apply_fields(&task, args)) {
        fputs("invalid task: check the title, due date, priority and category\n", stderr);
        return 1;
    }
    int id = list_add(list, task);
    if (id == 0) {
        fputs("invalid task: check the title, due date, priority and category\n", stderr);
        return 1;
    }
    printf("Added task %d.\n", id);
    return report_save(save_tasks(list));
}

static int cmd_edit(TaskList *list, const Args *args) {
    Task *task = find_task(list, args->positional);
    if (task == NULL) {
        fputs("no task with that id\n", stderr);
        return 1;
    }
    Task changed = *task;
    bool ok = apply_fields(&changed, args) &&
              (args->title == NULL || set_text(changed.title, sizeof changed.title, args->title)) &&
              task_is_valid(&changed);
    if (!ok) {
        fputs("invalid task: check the title, due date, priority and category\n", stderr);
        return 1;
    }
    *task = changed;
    printf("Updated task %d.\n", changed.id);
    return report_save(save_tasks(list));
}

static int cmd_set_done(TaskList *list, const Args *args, bool done) {
    Task *task = find_task(list, args->positional);
    if (task == NULL) {
        fputs("no task with that id\n", stderr);
        return 1;
    }
    task->done = done;
    printf("Task %d is now %s.\n", task->id, done ? "done" : "not done");
    return report_save(save_tasks(list));
}

static int cmd_remove(TaskList *list, const Args *args) {
    Task *task = find_task(list, args->positional);
    if (task == NULL) {
        fputs("no task with that id\n", stderr);
        return 1;
    }
    printf("Removed task %d.\n", task->id);
    list_remove(list, task->id);
    return report_save(save_tasks(list));
}

int main(int argc, char **argv) {
    const char *command = argc > 1 ? argv[1] : "list";
    if (strcmp(command, "-h") == 0 || strcmp(command, "--help") == 0) {
        print_usage(stdout);
        return 0;
    }

    Args args = {0};
    if (!parse_args(argc, argv, &args)) {
        print_usage(stderr);
        return 2;
    }

    TaskList list = {0};
    if (!load_tasks(&list)) {
        return 1;
    }

    if (strcmp(command, "list") == 0) {
        return cmd_list(&list, &args);
    }
    if (strcmp(command, "add") == 0) {
        return cmd_add(&list, &args);
    }
    if (strcmp(command, "edit") == 0) {
        return cmd_edit(&list, &args);
    }
    if (strcmp(command, "done") == 0) {
        return cmd_set_done(&list, &args, true);
    }
    if (strcmp(command, "undone") == 0) {
        return cmd_set_done(&list, &args, false);
    }
    if (strcmp(command, "rm") == 0) {
        return cmd_remove(&list, &args);
    }
    print_usage(stderr);
    return 2;
}
