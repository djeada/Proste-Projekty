"""Command line interface of the version control tool. Works on the current directory."""

import sys

import version_control as vcs

USAGE = """Usage: version_control <command> [argument]

Commands:
  init               create a repository in this directory
  commit "message"   save all files as a new commit
  log                list commits, newest first
  status             show files changed since the last commit
  diff [N]           show changed lines compared with commit N (default: last)
  checkout N         restore the files of commit N
"""


def parse_number(text):
    if not text.isascii() or not text.isdigit() or int(text) < 1:
        raise vcs.VcsError("Commit number must be a positive integer.")
    return int(text)


def show_log():
    count = vcs.commit_count(".")
    if count == 0:
        print("No commits yet.")
    for number in range(count, 0, -1):
        stamp, message = vcs.commit_info(".", number)
        print(f"#{number:<4} {vcs.format_time(stamp)}  {message}")


def show_status():
    count = vcs.commit_count(".")
    base = vcs.load_commit(".", count) if count else {}
    print("No commits yet." if count == 0 else f"Changes since commit #{count}:")
    found = vcs.changes(base, vcs.read_directory("."))
    if not found:
        print("  nothing changed")
    for kind, name in found:
        print(f"  {kind:<10} {name}")


def show_diff(params):
    count = vcs.commit_count(".")
    if count == 0:
        raise vcs.VcsError("No commits yet.")
    number = parse_number(params[0]) if params else count
    if number > count:
        raise vcs.VcsError(f"There is no commit #{number}.")

    base = vcs.load_commit(".", number)
    current = vcs.read_directory(".")
    found = vcs.changes(base, current)
    if not found:
        print(f"No changes since commit #{number}.")
    for kind, name in found:
        old_lines = [] if kind == "added" else vcs.split_lines(base[name])
        new_lines = [] if kind == "deleted" else vcs.split_lines(current[name])
        print(f"== {name} ({kind})")
        for tag, line in vcs.diff_lines(old_lines, new_lines):
            if tag != " ":
                print(tag + line)


def run(args):
    command, params = args[0], args[1:]
    if command == "init" and not params:
        vcs.init(".")
        print(f"Initialized empty repository in ./{vcs.VCS_DIR}")
    elif command == "commit" and len(params) == 1:
        print(f"Created commit #{vcs.commit('.', params[0])}")
    elif command == "log" and not params:
        show_log()
    elif command == "status" and not params:
        show_status()
    elif command == "diff" and len(params) <= 1:
        show_diff(params)
    elif command == "checkout" and len(params) == 1:
        number = parse_number(params[0])
        vcs.checkout(".", number)
        print(f"Restored the files of commit #{number}")
    else:
        print(USAGE, end="")
        return 1
    return 0


def main():
    if len(sys.argv) < 2:
        print(USAGE, end="")
        sys.exit(1)
    try:
        sys.exit(run(sys.argv[1:]))
    except vcs.VcsError as error:
        print(error)
        sys.exit(1)


if __name__ == "__main__":
    main()
