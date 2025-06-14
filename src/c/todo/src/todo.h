#ifndef TODO_H
#define TODO_H

#define MAX_TASKS 100
#define MAX_TASK_LEN 128

typedef struct {
    char tasks[MAX_TASKS][MAX_TASK_LEN];
    int count;
} TodoList;

void todo_init(TodoList *list);
void todo_add(TodoList *list, const char *task);
void todo_remove(TodoList *list, int index);
void todo_print(const TodoList *list);

#endif // TODO_H
