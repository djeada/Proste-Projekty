"""
Python implementation of a simple version control system.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.version_control.src.logic.repository import Repository


def main() -> None:
    repo = Repository(".")

    print("Version Control System")
    print("Commands: init, add <file>, commit <message>, log, checkout <id>, diff <id1> <id2>, status, quit")
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
        elif command == "init":
            if repo.init():
                print("Initialized empty repository.")
            else:
                print("Repository already initialized.")
        elif command == "add":
            if len(parts) < 2:
                print("Usage: add <filepath>")
            else:
                filepath = parts[1]
                if repo.add_file(filepath):
                    print(f"Added: {filepath}")
                else:
                    print(f"Could not add: {filepath}")
        elif command == "commit":
            if len(parts) < 2:
                print("Usage: commit <message>")
            else:
                message = parts[1]
                commit_id = repo.commit(message)
                if commit_id > 0:
                    print(f"Created commit #{commit_id}: {message}")
                else:
                    print("Could not create commit. Is the repository initialized?")
        elif command == "log":
            print(repo.log())
        elif command == "checkout":
            if len(parts) < 2:
                print("Usage: checkout <commit_id>")
            else:
                try:
                    commit_id = int(parts[1])
                    if repo.checkout(commit_id):
                        print(f"Checked out commit #{commit_id}")
                    else:
                        print("Could not checkout. Invalid commit ID?")
                except ValueError:
                    print("Please enter a valid commit ID")
        elif command == "diff":
            if len(parts) < 2:
                print("Usage: diff <commit_id1> <commit_id2>")
            else:
                try:
                    ids = parts[1].split()
                    if len(ids) >= 2:
                        id1, id2 = int(ids[0]), int(ids[1])
                        print(repo.diff(id1, id2))
                    else:
                        print("Usage: diff <commit_id1> <commit_id2>")
                except ValueError:
                    print("Please enter valid commit IDs")
        elif command == "status":
            print(repo.status())
        else:
            print(f"Unknown command: {command}")
            print("Commands: init, add <file>, commit <message>, log, checkout <id>, diff <id1> <id2>, status, quit")


if __name__ == "__main__":
    main()
