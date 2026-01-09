"""
Simple version control system logic.
"""
import os
import time
from typing import Optional, List, Dict
from dataclasses import dataclass, field
from datetime import datetime


@dataclass
class FileSnapshot:
    """Represents a snapshot of a file at a point in time."""

    path: str
    content: str
    size: int


@dataclass
class Commit:
    """Represents a commit."""

    id: int
    message: str
    timestamp: float
    files: Dict[str, FileSnapshot] = field(default_factory=dict)

    def get_timestamp_str(self) -> str:
        """Get formatted timestamp."""
        return datetime.fromtimestamp(self.timestamp).strftime("%Y-%m-%d %H:%M:%S")


class Repository:
    """
    Simple version control repository.
    """

    MAX_COMMITS = 100

    def __init__(self, path: str = ".") -> None:
        self.repo_path = os.path.abspath(path)
        self.commits: List[Commit] = []
        self.tracked_files: List[str] = []
        self.initialized = False

    def init(self) -> bool:
        """Initialize the repository."""
        if self.initialized:
            return False
        self.initialized = True
        return True

    def is_initialized(self) -> bool:
        """Check if repository is initialized."""
        return self.initialized

    def add_file(self, filepath: str) -> bool:
        """
        Add a file to be tracked.

        :param filepath: Path to the file (relative to repo path)
        :return: True if successful
        """
        if not self.initialized:
            return False

        full_path = os.path.join(self.repo_path, filepath)
        if not os.path.isfile(full_path):
            return False

        if filepath not in self.tracked_files:
            self.tracked_files.append(filepath)
        return True

    def commit(self, message: str) -> int:
        """
        Create a commit with current file states.

        :param message: Commit message
        :return: Commit ID or -1 if failed
        """
        if not self.initialized:
            return -1

        if len(self.commits) >= self.MAX_COMMITS:
            return -1

        commit_id = len(self.commits) + 1
        new_commit = Commit(
            id=commit_id,
            message=message,
            timestamp=time.time(),
        )

        for filepath in self.tracked_files:
            full_path = os.path.join(self.repo_path, filepath)
            if os.path.isfile(full_path):
                try:
                    with open(full_path, "r", encoding="utf-8") as f:
                        content = f.read()
                    new_commit.files[filepath] = FileSnapshot(
                        path=filepath,
                        content=content,
                        size=len(content),
                    )
                except (IOError, UnicodeDecodeError):
                    pass

        self.commits.append(new_commit)
        return commit_id

    def get_commit(self, commit_id: int) -> Optional[Commit]:
        """Get a commit by ID."""
        if commit_id < 1 or commit_id > len(self.commits):
            return None
        return self.commits[commit_id - 1]

    def get_commit_count(self) -> int:
        """Get number of commits."""
        return len(self.commits)

    def log(self) -> str:
        """Get formatted commit log."""
        if not self.commits:
            return "No commits yet."

        lines = []
        for commit in reversed(self.commits):
            lines.append(f"Commit #{commit.id}")
            lines.append(f"  Date: {commit.get_timestamp_str()}")
            lines.append(f"  Message: {commit.message}")
            lines.append(f"  Files: {len(commit.files)}")
            lines.append("")
        return "\n".join(lines)

    def checkout(self, commit_id: int) -> bool:
        """
        Restore files to a previous commit state.

        :param commit_id: The commit ID to restore
        :return: True if successful
        """
        commit = self.get_commit(commit_id)
        if not commit:
            return False

        for filepath, snapshot in commit.files.items():
            full_path = os.path.join(self.repo_path, filepath)
            try:
                os.makedirs(os.path.dirname(full_path), exist_ok=True)
                with open(full_path, "w", encoding="utf-8") as f:
                    f.write(snapshot.content)
            except IOError:
                return False

        return True

    def diff(self, commit_id1: int, commit_id2: int) -> str:
        """
        Show differences between two commits.

        :param commit_id1: First commit ID
        :param commit_id2: Second commit ID
        :return: Formatted diff string
        """
        commit1 = self.get_commit(commit_id1)
        commit2 = self.get_commit(commit_id2)

        if not commit1 or not commit2:
            return "Invalid commit ID(s)."

        lines = [f"Diff between commit #{commit_id1} and #{commit_id2}"]
        lines.append("")

        all_files = set(commit1.files.keys()) | set(commit2.files.keys())

        for filepath in sorted(all_files):
            file1 = commit1.files.get(filepath)
            file2 = commit2.files.get(filepath)

            if file1 and not file2:
                lines.append(f"- {filepath} (deleted)")
            elif file2 and not file1:
                lines.append(f"+ {filepath} (added)")
            elif file1 and file2 and file1.content != file2.content:
                lines.append(f"~ {filepath} (modified)")
                lines.append(f"    Size: {file1.size} -> {file2.size} bytes")

        if len(lines) == 2:
            lines.append("  No differences found.")

        return "\n".join(lines)

    def status(self) -> str:
        """Get current repository status."""
        if not self.initialized:
            return "Repository not initialized. Run 'init' first."

        lines = [f"Repository: {self.repo_path}"]
        lines.append(f"Commits: {len(self.commits)}")
        lines.append(f"Tracked files: {len(self.tracked_files)}")

        if self.tracked_files:
            lines.append("")
            lines.append("Files:")
            for filepath in self.tracked_files:
                full_path = os.path.join(self.repo_path, filepath)
                if os.path.isfile(full_path):
                    lines.append(f"  {filepath}")
                else:
                    lines.append(f"  {filepath} (missing)")

        return "\n".join(lines)
