"""Rules for tasks: validation, sorting, filtering, overdue check and the text file format."""

import re
from dataclasses import dataclass
from typing import List, Optional

PRIORITIES = ("low", "medium", "high")
MAX_TASKS = 200
MAX_TITLE_BYTES = 255
MAX_CATEGORY_BYTES = 63
FORBIDDEN_CHARS = ("\t", "\r", "\n")
DATE_PATTERN = re.compile(r"[0-9]{4}-[0-9]{2}-[0-9]{2}")


@dataclass(frozen=True)
class Task:
    id: int
    title: str
    priority: str = "medium"
    due: str = ""
    category: str = ""
    done: bool = False


def is_valid_date(text: str) -> bool:
    if not DATE_PATTERN.fullmatch(text):
        return False
    year, month, day = int(text[:4]), int(text[5:7]), int(text[8:])
    leap = year % 4 == 0 and (year % 100 != 0 or year % 400 == 0)
    days = [31, 29 if leap else 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    return 1 <= month <= 12 and 1 <= day <= days[month - 1]


def _is_valid_text(text: str, max_bytes: int, required: bool) -> bool:
    if required and text == "":
        return False
    if len(text.encode("utf-8")) > max_bytes:
        return False
    return not any(char in text for char in FORBIDDEN_CHARS)


def is_valid_task(task: Task) -> bool:
    return (
        _is_valid_text(task.title, MAX_TITLE_BYTES, required=True)
        and _is_valid_text(task.category, MAX_CATEGORY_BYTES, required=False)
        and task.priority in PRIORITIES
        and (task.due == "" or is_valid_date(task.due))
    )


def is_overdue(task: Task, today: str) -> bool:
    return not task.done and task.due != "" and task.due < today


def task_to_line(task: Task) -> str:
    fields = [str(task.id), "1" if task.done else "0", task.priority, task.due, task.category, task.title]
    return "\t".join(fields)


def task_from_line(line: str) -> Task:
    fields = line.rstrip("\r\n").split("\t")
    if len(fields) != 6:
        raise ValueError("expected 6 tab-separated fields")
    id_text, done, priority, due, category, title = fields
    if not id_text.isdigit() or done not in ("0", "1"):
        raise ValueError("bad id or done flag")
    task = Task(int(id_text), title, priority, due, category, done == "1")
    if task.id == 0 or not is_valid_task(task):
        raise ValueError("invalid task")
    return task


def next_id(tasks: List[Task]) -> int:
    return max((task.id for task in tasks), default=0) + 1


def find_task(tasks: List[Task], task_id: int) -> Optional[Task]:
    return next((task for task in tasks if task.id == task_id), None)


def sort_key(task: Task):
    # Active before done, dated before undated, earlier dates first, high priority first.
    return (task.done, task.due == "", task.due, -PRIORITIES.index(task.priority), task.id)


def visible_tasks(tasks: List[Task], category: Optional[str] = None, status: str = "all") -> List[Task]:
    result = []
    for task in tasks:
        if category is not None and task.category != category:
            continue
        if status == "active" and task.done:
            continue
        if status == "done" and not task.done:
            continue
        result.append(task)
    return sorted(result, key=sort_key)


def update_task(tasks: List[Task], changed: Task) -> List[Task]:
    return [changed if task.id == changed.id else task for task in tasks]


def remove_task(tasks: List[Task], task_id: int) -> List[Task]:
    return [task for task in tasks if task.id != task_id]
