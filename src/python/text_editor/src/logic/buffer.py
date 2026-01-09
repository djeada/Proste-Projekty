"""
Text buffer logic for text editor.
"""
from typing import Optional, List


class TextBuffer:
    """
    Text buffer for handling file operations and text manipulation.
    """

    MAX_LINES = 1000

    def __init__(self) -> None:
        self.lines: List[str] = []
        self.filename: str = ""
        self.modified: bool = False

    def load(self, filename: str) -> bool:
        """
        Load a file into the buffer.

        :param filename: Path to the file
        :return: True if successful, False otherwise
        """
        try:
            with open(filename, "r", encoding="utf-8") as f:
                self.lines = f.read().splitlines()
            self.filename = filename
            self.modified = False
            return True
        except (IOError, OSError):
            return False

    def save(self, filename: Optional[str] = None) -> bool:
        """
        Save the buffer to a file.

        :param filename: Path to save to (uses current filename if None)
        :return: True if successful, False otherwise
        """
        if filename:
            self.filename = filename
        if not self.filename:
            return False

        try:
            with open(self.filename, "w", encoding="utf-8") as f:
                f.write("\n".join(self.lines))
            self.modified = False
            return True
        except (IOError, OSError):
            return False

    def insert_line(self, pos: int, text: str) -> bool:
        """
        Insert a line at the specified position.

        :param pos: Position to insert at (0-based)
        :param text: Text to insert
        :return: True if successful
        """
        if pos < 0 or pos > len(self.lines):
            return False
        if len(self.lines) >= self.MAX_LINES:
            return False

        self.lines.insert(pos, text)
        self.modified = True
        return True

    def delete_line(self, pos: int) -> bool:
        """
        Delete a line at the specified position.

        :param pos: Position to delete (0-based)
        :return: True if successful
        """
        if pos < 0 or pos >= len(self.lines):
            return False

        self.lines.pop(pos)
        self.modified = True
        return True

    def append_line(self, text: str) -> bool:
        """
        Append a line to the end of the buffer.

        :param text: Text to append
        :return: True if successful
        """
        if len(self.lines) >= self.MAX_LINES:
            return False

        self.lines.append(text)
        self.modified = True
        return True

    def get_line(self, pos: int) -> str:
        """
        Get a line at the specified position.

        :param pos: Position (0-based)
        :return: The line content or empty string if invalid
        """
        if pos < 0 or pos >= len(self.lines):
            return ""
        return self.lines[pos]

    def set_line(self, pos: int, text: str) -> bool:
        """
        Set the content of a line.

        :param pos: Position (0-based)
        :param text: New text content
        :return: True if successful
        """
        if pos < 0 or pos >= len(self.lines):
            return False
        self.lines[pos] = text
        self.modified = True
        return True

    def line_count(self) -> int:
        """Get the number of lines in the buffer."""
        return len(self.lines)

    def get_content(self) -> str:
        """Get the full content of the buffer."""
        return "\n".join(self.lines)

    def set_content(self, content: str) -> None:
        """Set the full content of the buffer."""
        self.lines = content.splitlines()
        self.modified = True

    def clear(self) -> None:
        """Clear the buffer."""
        self.lines = []
        self.filename = ""
        self.modified = False

    def is_modified(self) -> bool:
        """Check if the buffer has been modified."""
        return self.modified
