"""
Todo list logic.
"""
from typing import List


class TodoList:
    """
    Simple todo list manager.
    """

    MAX_TASKS = 100
    MAX_TASK_LEN = 128

    def __init__(self) -> None:
        self.tasks: List[str] = []

    def add(self, task: str) -> bool:
        """
        Add a task to the list.

        :param task: The task description
        :return: True if added successfully, False if list is full
        """
        if len(self.tasks) >= self.MAX_TASKS:
            return False

        # Truncate task if too long
        if len(task) > self.MAX_TASK_LEN:
            task = task[: self.MAX_TASK_LEN]

        self.tasks.append(task)
        return True

    def remove(self, index: int) -> bool:
        """
        Remove a task by index.

        :param index: The 0-based index of the task to remove
        :return: True if removed successfully, False if index is invalid
        """
        if index < 0 or index >= len(self.tasks):
            return False

        self.tasks.pop(index)
        return True

    def get(self, index: int) -> str:
        """
        Get a task by index.

        :param index: The 0-based index of the task
        :return: The task string, or empty string if invalid index
        """
        if index < 0 or index >= len(self.tasks):
            return ""
        return self.tasks[index]

    def count(self) -> int:
        """
        Get the number of tasks.

        :return: Number of tasks in the list
        """
        return len(self.tasks)

    def print_all(self) -> str:
        """
        Get a formatted string of all tasks.

        :return: Formatted string with all tasks
        """
        if not self.tasks:
            return "No tasks."

        lines = []
        for i, task in enumerate(self.tasks):
            lines.append(f"{i + 1}: {task}")
        return "\n".join(lines)

    def clear(self) -> None:
        """Clear all tasks."""
        self.tasks.clear()
