/* Tests must always run, even in a Release build where NDEBUG removes assert(). */
#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "todo.h"

static Task make_task(const char *title, Priority priority, const char *due, bool done) {
    Task task = {0};
    strcpy(task.title, title);
    task.priority = priority;
    strcpy(task.due, due);
    task.done = done;
    return task;
}

static void test_dates(void) {
    assert(is_valid_date("2026-10-20"));
    assert(is_valid_date("2028-02-29"));
    assert(!is_valid_date("2026-02-29"));
    assert(!is_valid_date("2026-13-01"));
    assert(!is_valid_date("2026-04-31"));
    assert(!is_valid_date("2026-1-5"));
    assert(!is_valid_date("20-10-2026"));
    assert(!is_valid_date("tomorrow"));
}

static void test_task_validation(void) {
    Task task = make_task("Buy milk", PRIORITY_HIGH, "2026-10-20", false);
    assert(task_is_valid(&task));

    Task no_title = make_task("", PRIORITY_LOW, "", false);
    assert(!task_is_valid(&no_title));

    Task tab_in_title = make_task("Buy\tmilk", PRIORITY_LOW, "", false);
    assert(!task_is_valid(&tab_in_title));

    Task bad_date = make_task("Buy milk", PRIORITY_LOW, "2026-02-30", false);
    assert(!task_is_valid(&bad_date));
}

static void test_serialization(void) {
    Task task = make_task("Buy milk", PRIORITY_HIGH, "2026-10-20", true);
    strcpy(task.category, "home");
    task.id = 3;

    char line[LINE_LEN];
    bool written = task_to_line(&task, line, sizeof line);
    assert(written);
    assert(strcmp(line, "3\t1\thigh\t2026-10-20\thome\tBuy milk") == 0);

    Task parsed;
    bool read = task_from_line(line, &parsed);
    assert(read);
    assert(parsed.id == 3 && parsed.done && parsed.priority == PRIORITY_HIGH);
    assert(strcmp(parsed.due, "2026-10-20") == 0 && strcmp(parsed.category, "home") == 0);
    assert(strcmp(parsed.title, "Buy milk") == 0);

    Task bad;
    assert(!task_from_line("3\t1\thigh\t2026-10-20\thome", &bad));
    assert(!task_from_line("3\t2\thigh\t\t\tBuy milk", &bad));
    assert(!task_from_line("3\t0\turgent\t\t\tBuy milk", &bad));
    assert(!task_from_line("x\t0\tlow\t\t\tBuy milk", &bad));
    assert(!task_from_line("3\t0\tlow\t2026-02-30\t\tBuy milk", &bad));
}

static void test_overdue(void) {
    Task late = make_task("Pay rent", PRIORITY_LOW, "2026-10-01", false);
    assert(task_is_overdue(&late, "2026-10-08"));

    Task due_today = make_task("Pay rent", PRIORITY_LOW, "2026-10-08", false);
    assert(!task_is_overdue(&due_today, "2026-10-08"));

    Task finished = make_task("Pay rent", PRIORITY_LOW, "2026-10-01", true);
    assert(!task_is_overdue(&finished, "2026-10-08"));

    Task no_date = make_task("Read", PRIORITY_LOW, "", false);
    assert(!task_is_overdue(&no_date, "2026-10-08"));
}

static void test_add_find_remove(void) {
    TaskList list = {0};
    Task first = make_task("First", PRIORITY_LOW, "", false);
    Task second = make_task("Second", PRIORITY_HIGH, "", false);
    int id1 = list_add(&list, first);
    int id2 = list_add(&list, second);
    assert(id1 == 1 && id2 == 2);

    Task invalid = make_task("", PRIORITY_LOW, "", false);
    assert(list_add(&list, invalid) == 0);
    assert(list.count == 2);

    Task *found = list_find(&list, 2);
    assert(found != NULL && strcmp(found->title, "Second") == 0);
    assert(list_find(&list, 9) == NULL);

    bool removed = list_remove(&list, 1);
    assert(removed && list.count == 1 && list.tasks[0].id == 2);
    assert(!list_remove(&list, 1));
    assert(list_add(&list, first) == 3);
}

static void test_sorting(void) {
    TaskList list = {0};
    Task tasks[] = {
        make_task("No date low", PRIORITY_LOW, "", false),
        make_task("Later high", PRIORITY_HIGH, "2026-12-01", false),
        make_task("Soon low", PRIORITY_LOW, "2026-10-09", false),
        make_task("Soon high", PRIORITY_HIGH, "2026-10-09", false),
        make_task("Finished", PRIORITY_HIGH, "2026-01-01", true),
        make_task("No date high", PRIORITY_HIGH, "", false),
    };
    for (size_t i = 0; i < sizeof tasks / sizeof tasks[0]; ++i) {
        list_add(&list, tasks[i]);
    }

    const Task *views[MAX_TASKS];
    size_t count = list_view(&list, NULL, STATUS_ALL, views);
    assert(count == 6);
    const char *expected[] = {"Soon high",  "Soon low",    "Later high",
                              "No date high", "No date low", "Finished"};
    for (size_t i = 0; i < count; ++i) {
        assert(strcmp(views[i]->title, expected[i]) == 0);
    }
}

static void test_filtering(void) {
    TaskList list = {0};
    Task work = make_task("Report", PRIORITY_HIGH, "", false);
    strcpy(work.category, "work");
    Task home = make_task("Dishes", PRIORITY_LOW, "", true);
    strcpy(home.category, "home");
    list_add(&list, work);
    list_add(&list, home);

    const Task *views[MAX_TASKS];
    assert(list_view(&list, "work", STATUS_ALL, views) == 1);
    assert(list_view(&list, "home", STATUS_ACTIVE, views) == 0);
    assert(list_view(&list, NULL, STATUS_ACTIVE, views) == 1);

    size_t done_count = list_view(&list, NULL, STATUS_DONE, views);
    assert(done_count == 1 && strcmp(views[0]->title, "Dishes") == 0);
}

int main(void) {
    test_dates();
    test_task_validation();
    test_serialization();
    test_overdue();
    test_add_find_remove();
    test_sorting();
    test_filtering();
    puts("All todo tests passed.");
    return 0;
}
