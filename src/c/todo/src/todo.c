#include "todo.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *const PRIORITY_NAMES[] = {"low", "medium", "high"};

static int days_in_month(int year, int month) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    if (month == 2 && leap) {
        return 29;
    }
    return days[month - 1];
}

bool is_valid_date(const char *text) {
    if (strlen(text) != DATE_LEN - 1) {
        return false;
    }
    for (int i = 0; i < DATE_LEN - 1; ++i) {
        bool separator = (i == 4 || i == 7);
        if (separator ? text[i] != '-' : !isdigit((unsigned char)text[i])) {
            return false;
        }
    }
    int year = 0;
    int month = 0;
    int day = 0;
    sscanf(text, "%4d-%2d-%2d", &year, &month, &day);
    if (month < 1 || month > 12) {
        return false;
    }
    return day >= 1 && day <= days_in_month(year, month);
}

static bool is_valid_text(const char *text, size_t max_len, bool required) {
    size_t len = strlen(text);
    if (len > max_len || (required && len == 0)) {
        return false;
    }
    return strpbrk(text, "\t\r\n") == NULL;
}

const char *priority_name(Priority priority) {
    return PRIORITY_NAMES[priority];
}

bool parse_priority(const char *text, Priority *priority) {
    for (int i = 0; i <= PRIORITY_HIGH; ++i) {
        if (strcmp(text, PRIORITY_NAMES[i]) == 0) {
            *priority = (Priority)i;
            return true;
        }
    }
    return false;
}

bool task_is_valid(const Task *task) {
    if (!is_valid_text(task->title, MAX_TITLE - 1, true)) {
        return false;
    }
    if (!is_valid_text(task->category, MAX_CATEGORY - 1, false)) {
        return false;
    }
    if (task->due[0] != '\0' && !is_valid_date(task->due)) {
        return false;
    }
    return task->priority >= PRIORITY_LOW && task->priority <= PRIORITY_HIGH;
}

bool task_is_overdue(const Task *task, const char *today) {
    return !task->done && task->due[0] != '\0' && strcmp(task->due, today) < 0;
}

bool task_to_line(const Task *task, char *buf, size_t size) {
    int written = snprintf(buf, size, "%d\t%d\t%s\t%s\t%s\t%s", task->id, task->done ? 1 : 0,
                           priority_name(task->priority), task->due, task->category, task->title);
    return written >= 0 && (size_t)written < size;
}

static bool all_digits(const char *text) {
    if (text[0] == '\0') {
        return false;
    }
    for (const char *p = text; *p != '\0'; ++p) {
        if (!isdigit((unsigned char)*p)) {
            return false;
        }
    }
    return true;
}

static bool copy_field(char *dst, size_t size, const char *src) {
    if (strlen(src) >= size) {
        return false;
    }
    strcpy(dst, src);
    return true;
}

bool task_from_line(const char *line, Task *task) {
    char buf[LINE_LEN];
    if (strlen(line) >= sizeof buf) {
        return false;
    }
    strcpy(buf, line);
    buf[strcspn(buf, "\r\n")] = '\0';

    char *fields[6] = {buf};
    int count = 1;
    for (char *p = buf; *p != '\0' && count < 6; ++p) {
        if (*p == '\t') {
            *p = '\0';
            fields[count++] = p + 1;
        }
    }
    if (count != 6 || !all_digits(fields[0]) || strlen(fields[0]) > 9) {
        return false;
    }
    task->id = atoi(fields[0]);

    if (strcmp(fields[1], "1") == 0) {
        task->done = true;
    } else if (strcmp(fields[1], "0") == 0) {
        task->done = false;
    } else {
        return false;
    }

    return task->id > 0 && parse_priority(fields[2], &task->priority) &&
           copy_field(task->due, sizeof task->due, fields[3]) &&
           copy_field(task->category, sizeof task->category, fields[4]) &&
           copy_field(task->title, sizeof task->title, fields[5]) && task_is_valid(task);
}

int list_add(TaskList *list, Task task) {
    if (list->count >= MAX_TASKS || !task_is_valid(&task)) {
        return 0;
    }
    int max_id = 0;
    for (int i = 0; i < list->count; ++i) {
        if (list->tasks[i].id > max_id) {
            max_id = list->tasks[i].id;
        }
    }
    task.id = max_id + 1;
    list->tasks[list->count++] = task;
    return task.id;
}

Task *list_find(TaskList *list, int id) {
    for (int i = 0; i < list->count; ++i) {
        if (list->tasks[i].id == id) {
            return &list->tasks[i];
        }
    }
    return NULL;
}

bool list_remove(TaskList *list, int id) {
    for (int i = 0; i < list->count; ++i) {
        if (list->tasks[i].id == id) {
            size_t after = (size_t)(list->count - i - 1);
            memmove(&list->tasks[i], &list->tasks[i + 1], after * sizeof(Task));
            list->count--;
            return true;
        }
    }
    return false;
}

static bool matches(const Task *task, const char *category, StatusFilter status) {
    if (category != NULL && strcmp(task->category, category) != 0) {
        return false;
    }
    if (status == STATUS_ACTIVE) {
        return !task->done;
    }
    if (status == STATUS_DONE) {
        return task->done;
    }
    return true;
}

/* Tasks without a due date go after all tasks with one. */
static int compare_due(const char *a, const char *b) {
    if (a[0] == '\0' || b[0] == '\0') {
        return (a[0] == '\0') - (b[0] == '\0');
    }
    return strcmp(a, b);
}

static int compare_views(const void *pa, const void *pb) {
    const Task *a = *(const Task *const *)pa;
    const Task *b = *(const Task *const *)pb;
    if (a->done != b->done) {
        return a->done - b->done;
    }
    int by_due = compare_due(a->due, b->due);
    if (by_due != 0) {
        return by_due;
    }
    if (a->priority != b->priority) {
        return (int)b->priority - (int)a->priority;
    }
    return a->id - b->id;
}

size_t list_view(const TaskList *list, const char *category, StatusFilter status,
                 const Task *views[]) {
    size_t count = 0;
    for (int i = 0; i < list->count; ++i) {
        if (matches(&list->tasks[i], category, status)) {
            views[count++] = &list->tasks[i];
        }
    }
    qsort(views, count, sizeof views[0], compare_views);
    return count;
}
