"""
Python implementation of a simple text editor.
"""
from src.python.text_editor.src.gui.gui import Gui
from src.python.text_editor.src.logic.buffer import TextBuffer


def main() -> None:
    buffer = TextBuffer()
    gui = Gui(buffer)
    gui.run()


if __name__ == "__main__":
    main()
