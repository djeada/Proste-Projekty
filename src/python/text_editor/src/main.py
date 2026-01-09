"""
Python implementation of a simple text editor.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.text_editor.src.gui.gui import Gui
from src.python.text_editor.src.logic.buffer import TextBuffer


def main() -> None:
    buffer = TextBuffer()
    gui = Gui(buffer)
    gui.run()


if __name__ == "__main__":
    main()
