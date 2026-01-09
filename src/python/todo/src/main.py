"""
Python implementation of a simple todo list application.

Commands:
- add <task>: Add a new task
- remove <index>: Remove task at index
- list: Show all tasks
- quit: Exit the application
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.todo.src.logic.todo_list import TodoList


def main() -> None:
    todo = TodoList()
    print("Todo List Application")
    print("Commands: add <task>, remove <index>, list, quit")
    print()

    while True:
        try:
            user_input = input("> ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\nGoodbye!")
            break

        if not user_input:
            continue

        parts = user_input.split(maxsplit=1)
        command = parts[0].lower()

        if command == "quit" or command == "q":
            print("Goodbye!")
            break
        elif command == "add":
            if len(parts) < 2:
                print("Usage: add <task>")
            else:
                task = parts[1]
                if todo.add(task):
                    print(f"Added: {task}")
                else:
                    print("Task list is full!")
        elif command == "remove" or command == "rm":
            if len(parts) < 2:
                print("Usage: remove <index>")
            else:
                try:
                    index = int(parts[1]) - 1  # 1-based to 0-based
                    if todo.remove(index):
                        print(f"Removed task {index + 1}")
                    else:
                        print("Invalid index!")
                except ValueError:
                    print("Please enter a valid number")
        elif command == "list" or command == "ls":
            print(todo.print_all())
        elif command == "clear":
            todo.clear()
            print("All tasks cleared.")
        else:
            print(f"Unknown command: {command}")
            print("Commands: add <task>, remove <index>, list, quit")


if __name__ == "__main__":
    main()
