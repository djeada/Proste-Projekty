"""Terminal interface: parses the command line and keeps the tasks in tasks.txt."""

import argparse
import sys
from dataclasses import replace
from datetime import date
from pathlib import Path
from typing import List, Optional

from todo import (
    MAX_TASKS,
    PRIORITIES,
    Task,
    find_task,
    is_overdue,
    is_valid_task,
    next_id,
    remove_task,
    task_from_line,
    task_to_line,
    update_task,
    visible_tasks,
)

STORAGE = Path("tasks.txt")
RED = "\033[31m"
RESET = "\033[0m"
INVALID = "invalid task: check the title, due date, priority and category"


def fail(message: str) -> None:
    raise SystemExit(message)


def load_tasks(path: Path) -> List[Task]:
    if not path.exists():
        return []
    tasks = []
    with path.open(encoding="utf-8") as file:
        for number, line in enumerate(file, start=1):
            if line.rstrip("\r\n") == "":
                continue
            try:
                tasks.append(task_from_line(line))
            except ValueError:
                fail(f"{path}: line {number} is not a valid task")
    if len(tasks) > MAX_TASKS:
        fail(f"{path}: more than {MAX_TASKS} tasks")
    return tasks


def save_tasks(path: Path, tasks: List[Task]) -> None:
    path.write_text("".join(task_to_line(task) + "\n" for task in tasks), encoding="utf-8")


def empty_if_none(value: Optional[str]) -> str:
    return "" if value in (None, "none") else value


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(prog="todo", description="A simple task list kept in tasks.txt.")
    commands = parser.add_subparsers(dest="command", required=True)

    list_cmd = commands.add_parser("list", help="show the tasks")
    list_cmd.add_argument("--category")
    list_cmd.add_argument("--status", choices=["all", "active", "done"], default="all")

    add = commands.add_parser("add", help="add a task")
    add.add_argument("title")
    add.add_argument("--due", help="YYYY-MM-DD")
    add.add_argument("--priority", choices=PRIORITIES, default="medium")
    add.add_argument("--category")

    edit = commands.add_parser("edit", help="change a task; 'none' clears a date or category")
    edit.add_argument("id", type=int)
    edit.add_argument("--title")
    edit.add_argument("--due", help="YYYY-MM-DD or none")
    edit.add_argument("--priority", choices=PRIORITIES)
    edit.add_argument("--category", help="or none")

    for name, text in (("done", "mark a task as done"), ("undone", "mark a task as not done"),
                       ("rm", "delete a task")):
        commands.add_parser(name, help=text).add_argument("id", type=int)
    return parser


def cmd_list(tasks: List[Task], args: argparse.Namespace, today: str) -> None:
    views = visible_tasks(tasks, args.category, args.status)
    if not views:
        print("No tasks.")
        return
    print(f"{'ID':<4} {'STATUS':<6} {'PRIORITY':<8} {'DUE':<10} {'CATEGORY':<12} TITLE")
    for task in views:
        overdue = is_overdue(task, today)
        mark = "[x]" if task.done else "[!]" if overdue else "[ ]"
        line = (f"{task.id:<4} {mark:<6} {task.priority:<8} {task.due or '-':<10} "
                f"{task.category or '-':<12} {task.title}")
        print(f"{RED}{line}{RESET}" if overdue else line)


def cmd_add(tasks: List[Task], args: argparse.Namespace) -> List[Task]:
    if len(tasks) >= MAX_TASKS:
        fail("the list is full")
    task = Task(next_id(tasks), args.title, args.priority,
                empty_if_none(args.due), empty_if_none(args.category))
    if not is_valid_task(task):
        fail(INVALID)
    print(f"Added task {task.id}.")
    return tasks + [task]


def cmd_edit(tasks: List[Task], args: argparse.Namespace) -> List[Task]:
    task = find_task(tasks, args.id)
    if task is None:
        fail("no task with that id")
    changes = {}
    if args.title is not None:
        changes["title"] = args.title
    if args.due is not None:
        changes["due"] = empty_if_none(args.due)
    if args.priority is not None:
        changes["priority"] = args.priority
    if args.category is not None:
        changes["category"] = empty_if_none(args.category)
    changed = replace(task, **changes)
    if not is_valid_task(changed):
        fail(INVALID)
    print(f"Updated task {task.id}.")
    return update_task(tasks, changed)


def cmd_set_done(tasks: List[Task], args: argparse.Namespace) -> List[Task]:
    task = find_task(tasks, args.id)
    if task is None:
        fail("no task with that id")
    done = args.command == "done"
    print(f"Task {task.id} is now {'done' if done else 'not done'}.")
    return update_task(tasks, replace(task, done=done))


def cmd_remove(tasks: List[Task], args: argparse.Namespace) -> List[Task]:
    if find_task(tasks, args.id) is None:
        fail("no task with that id")
    print(f"Removed task {args.id}.")
    return remove_task(tasks, args.id)


def main() -> None:
    args = build_parser().parse_args(sys.argv[1:] or ["list"])
    tasks = load_tasks(STORAGE)
    today = date.today().isoformat()

    if args.command == "list":
        cmd_list(tasks, args, today)
        return
    if args.command == "add":
        tasks = cmd_add(tasks, args)
    elif args.command == "edit":
        tasks = cmd_edit(tasks, args)
    elif args.command in ("done", "undone"):
        tasks = cmd_set_done(tasks, args)
    else:
        tasks = cmd_remove(tasks, args)
    save_tasks(STORAGE, tasks)


if __name__ == "__main__":
    main()
