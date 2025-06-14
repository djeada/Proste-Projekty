#include "../src/todo.h"
#include <assert.h>
#include <stdio.h>

void test_todo_add_and_remove() {
    TodoList list;
    todo_init(&list);
    todo_add(&list, "Task 1");
    todo_add(&list, "Task 2");
    assert(list.count == 2);
    assert(strcmp(list.tasks[0], "Task 1") == 0);
    todo_remove(&list, 0);
    assert(list.count == 1);
    assert(strcmp(list.tasks[0], "Task 2") == 0);
}

int main() {
    test_todo_add_and_remove();
    printf("Todo logic tests passed.\n");
    return 0;
}
