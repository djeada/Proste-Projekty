from dataclasses import replace

import pytest

from todo import (
    Task,
    is_overdue,
    is_valid_date,
    is_valid_task,
    next_id,
    remove_task,
    task_from_line,
    task_to_line,
    update_task,
    visible_tasks,
)

TODAY = "2026-10-08"


def test_dates_are_checked_in_the_calendar():
    assert is_valid_date("2026-10-20")
    assert is_valid_date("2028-02-29")
    assert not is_valid_date("2026-02-29")
    assert not is_valid_date("2100-02-29")
    assert not is_valid_date("2026-13-01")
    assert not is_valid_date("2026-04-31")
    assert not is_valid_date("20-10-2026")
    assert not is_valid_date("2026-1-5")


@pytest.mark.parametrize(
    "task",
    [
        Task(1, ""),
        Task(1, "Buy\tmilk"),
        Task(1, "Buy milk", category="home\nwork"),
        Task(1, "Buy milk", priority="urgent"),
        Task(1, "Buy milk", due="2026-02-30"),
        Task(1, "x" * 256),
    ],
)
def test_invalid_tasks_are_rejected(task):
    assert not is_valid_task(task)


def test_valid_task_is_accepted():
    assert is_valid_task(Task(1, "Buy milk", "high", "2026-10-20", "home"))


def test_line_format_round_trip():
    task = Task(3, "Buy milk", "high", "2026-10-20", "home", done=True)
    line = task_to_line(task)
    assert line == "3\t1\thigh\t2026-10-20\thome\tBuy milk"
    assert task_from_line(line + "\n") == task


def test_line_without_due_date_and_category():
    assert task_from_line("4\t0\tlow\t\t\tRead") == Task(4, "Read", "low", "", "", False)


@pytest.mark.parametrize(
    "line",
    [
        "3\t1\thigh\t2026-10-20\thome",
        "3\t2\thigh\t\t\tBuy milk",
        "3\t0\turgent\t\t\tBuy milk",
        "x\t0\tlow\t\t\tBuy milk",
        "0\t0\tlow\t\t\tBuy milk",
        "3\t0\tlow\t2026-02-30\t\tBuy milk",
    ],
)
def test_bad_lines_are_rejected(line):
    with pytest.raises(ValueError):
        task_from_line(line)


def test_overdue_means_not_done_and_due_before_today():
    assert is_overdue(Task(1, "Pay rent", due="2026-10-01"), TODAY)
    assert not is_overdue(Task(1, "Pay rent", due="2026-10-08"), TODAY)
    assert not is_overdue(Task(1, "Pay rent", due="2026-10-01", done=True), TODAY)
    assert not is_overdue(Task(1, "Read"), TODAY)


def test_sorting_puts_active_dated_and_high_priority_first():
    tasks = [
        Task(1, "No date low", "low"),
        Task(2, "Later high", "high", "2026-12-01"),
        Task(3, "Soon low", "low", "2026-10-09"),
        Task(4, "Soon high", "high", "2026-10-09"),
        Task(5, "Finished", "high", "2026-01-01", done=True),
        Task(6, "No date high", "high"),
    ]
    titles = [task.title for task in visible_tasks(tasks)]
    assert titles == [
        "Soon high",
        "Soon low",
        "Later high",
        "No date high",
        "No date low",
        "Finished",
    ]


def test_filter_by_category_and_status():
    tasks = [
        Task(1, "Report", category="work"),
        Task(2, "Dishes", category="home", done=True),
    ]
    assert [t.title for t in visible_tasks(tasks, category="work")] == ["Report"]
    assert visible_tasks(tasks, category="home", status="active") == []
    assert [t.title for t in visible_tasks(tasks, status="done")] == ["Dishes"]
    assert len(visible_tasks(tasks, status="all")) == 2


def test_ids_update_and_remove():
    tasks = [Task(1, "First"), Task(7, "Second")]
    assert next_id(tasks) == 8
    assert next_id([]) == 1

    changed = update_task(tasks, replace(tasks[0], done=True))
    assert changed[0].done and changed[1] == tasks[1]
    assert [t.id for t in remove_task(tasks, 1)] == [7]
