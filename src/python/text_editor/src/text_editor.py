"""Text helpers of the editor: search, positions and counts. No tkinter here."""


def find_next(text, needle, start):
    """Return the index of the first match at or after start, wrapping to the beginning.

    Returns -1 when there is no match or the needle is empty.
    """
    if not needle:
        return -1
    index = text.find(needle, start)
    if index == -1:
        index = text.find(needle)
    return index


def line_col(text, offset):
    """Return the 1-based (line, column) of the character at offset."""
    before = text[:offset]
    line = before.count("\n") + 1
    column = offset - (before.rfind("\n") + 1) + 1
    return line, column


def count_lines(text):
    """Count lines the way the C version does: a final newline does not start a new line."""
    return text.count("\n") + (0 if text.endswith("\n") else 1)


def count_words(text):
    return len(text.split())
