"""Logic of the version control tool: commits, checkout, status and diffs."""

import os
import time

VCS_DIR = ".vcs"
MAX_MESSAGE_BYTES = 254


class VcsError(Exception):
    """A problem the user should see as a message."""


def _commit_dir(directory, number):
    return os.path.join(directory, VCS_DIR, "commits", str(number))


def repo_exists(directory):
    return os.path.isdir(os.path.join(directory, VCS_DIR))


def init(directory):
    if repo_exists(directory):
        raise VcsError("Could not create a repository (already initialized?).")
    os.makedirs(os.path.join(directory, VCS_DIR, "commits"))


def commit_count(directory):
    if not repo_exists(directory):
        raise VcsError("Not a repository. Run 'version_control init' first.")
    count = 0
    while os.path.isfile(os.path.join(_commit_dir(directory, count + 1), "meta")):
        count += 1
    return count


def read_directory(directory):
    """Return {file name: bytes} for the regular files directly inside a directory."""
    files = {}
    with os.scandir(directory) as entries:
        for entry in entries:
            if entry.is_file(follow_symlinks=False):
                with open(entry.path, "rb") as f:
                    files[entry.name] = f.read()
    return files


def commit(directory, message):
    """Save the files of the directory as a new commit and return its number."""
    if "\n" in message or len(message.encode("utf-8")) > MAX_MESSAGE_BYTES:
        raise VcsError(f"Commit failed. The message must be one line and shorter than {MAX_MESSAGE_BYTES + 1} bytes.")
    number = commit_count(directory) + 1
    folder = _commit_dir(directory, number)
    os.makedirs(os.path.join(folder, "files"))
    for name, data in read_directory(directory).items():
        with open(os.path.join(folder, "files", name), "wb") as f:
            f.write(data)
    with open(os.path.join(folder, "meta"), "w", encoding="utf-8") as f:
        f.write(f"{int(time.time())}\n{message}\n")
    return number


def commit_info(directory, number):
    """Return (unix time, message) of a commit."""
    with open(os.path.join(_commit_dir(directory, number), "meta"), encoding="utf-8") as f:
        stamp = f.readline()
        message = f.readline()
    return int(stamp), message.rstrip("\n")


def load_commit(directory, number):
    """Return {file name: bytes} stored in a commit."""
    folder = os.path.join(_commit_dir(directory, number), "files")
    if not os.path.isdir(folder):
        raise VcsError(f"There is no commit #{number}.")
    return read_directory(folder)


def checkout(directory, number):
    """Write the files of a commit into the directory. Existing files with the same name are overwritten."""
    for name, data in load_commit(directory, number).items():
        with open(os.path.join(directory, name), "wb") as f:
            f.write(data)


def changes(base, current):
    """Compare two snapshots. Return a list of (kind, name), kind is added, modified or deleted."""
    result = []
    for name in sorted(current):
        if name not in base:
            result.append(("added", name))
        elif base[name] != current[name]:
            result.append(("modified", name))
    for name in sorted(base):
        if name not in current:
            result.append(("deleted", name))
    return result


def split_lines(data):
    lines = data.decode("utf-8", errors="replace").split("\n")
    if lines[-1] == "":
        lines.pop()
    return lines


def diff_lines(old_lines, new_lines):
    """Return (tag, line) pairs: ' ' for a common line, '-' for a removed one, '+' for an added one.

    The common lines are found with a longest common subsequence table, lcs[i][j] being the
    length of the longest common subsequence of old_lines[i:] and new_lines[j:].
    """
    n, m = len(old_lines), len(new_lines)
    lcs = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(n - 1, -1, -1):
        for j in range(m - 1, -1, -1):
            if old_lines[i] == new_lines[j]:
                lcs[i][j] = lcs[i + 1][j + 1] + 1
            else:
                lcs[i][j] = max(lcs[i + 1][j], lcs[i][j + 1])

    result = []
    i = j = 0
    while i < n or j < m:
        if i < n and j < m and old_lines[i] == new_lines[j]:
            result.append((" ", old_lines[i]))
            i += 1
            j += 1
        elif i < n and (j == m or lcs[i + 1][j] >= lcs[i][j + 1]):
            result.append(("-", old_lines[i]))
            i += 1
        else:
            result.append(("+", new_lines[j]))
            j += 1
    return result


def format_time(stamp):
    return time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(stamp))
