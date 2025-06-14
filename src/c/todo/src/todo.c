#include "todo.h"
#include <string.h>
#include <stdio.h>

void todo_init(TodoList *list) {
    list->count = 0;
}

void todo_add(TodoList *list, const char *task) {
    if (list->count < MAX_TASKS) {
        strncpy(list->tasks[list->count], task, MAX_TASK_LEN - 1);
        list->tasks[list->count][MAX_TASK_LEN - 1] = '\0';
        list->count++;
    }
}

void todo_remove(TodoList *list, int index) {
    if (index >= 0 && index < list->count) {
        for (int i = index; i < list->count - 1; ++i) {
            strncpy(list->tasks[i], list->tasks[i + 1], MAX_TASK_LEN);
        }
        list->count--;
    }
}

void todo_print(const TodoList *list) {
    for (int i = 0; i < list->count; ++i) {
        printf("%d: %s\n", i + 1, list->tasks[i]);
    }
}
