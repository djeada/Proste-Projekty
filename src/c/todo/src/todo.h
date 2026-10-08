/* Rules for tasks: validation, sorting, filtering, overdue check and the text file format. */
#ifndef TODO_H
#define TODO_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_TASKS 200
#define MAX_TITLE 256    /* up to 255 bytes */
#define MAX_CATEGORY 64  /* up to 63 bytes */
#define DATE_LEN 11      /* YYYY-MM-DD and the terminating '\0' */
#define LINE_LEN 1024    /* longest line that task_to_line can write */

typedef enum { PRIORITY_LOW, PRIORITY_MEDIUM, PRIORITY_HIGH } Priority;
typedef enum { STATUS_ALL, STATUS_ACTIVE, STATUS_DONE } StatusFilter;

typedef struct {
    int id;
    char title[MAX_TITLE];
    Priority priority;
    char due[DATE_LEN];           /* empty string when there is no due date */
    char category[MAX_CATEGORY];  /* empty string when there is no category */
    bool done;
} Task;

typedef struct {
    Task tasks[MAX_TASKS];
    int count;
} TaskList;

bool is_valid_date(const char *text);
const char *priority_name(Priority priority);
bool parse_priority(const char *text, Priority *priority);
bool task_is_valid(const Task *task);
bool task_is_overdue(const Task *task, const char *today);

/* Writes "id<TAB>done<TAB>priority<TAB>due<TAB>category<TAB>title" (no newline). */
bool task_to_line(const Task *task, char *buf, size_t size);
bool task_from_line(const char *line, Task *task);

/* Returns the new id (1, 2, ...), or 0 when the list is full or the task is invalid. */
int list_add(TaskList *list, Task task);
Task *list_find(TaskList *list, int id);
bool list_remove(TaskList *list, int id);

/* Fills views with the matching tasks sorted for display; category NULL means any category. */
size_t list_view(const TaskList *list, const char *category, StatusFilter status,
                 const Task *views[]);

#endif
