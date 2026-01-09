"""
Python implementation of a simple graphics editor.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.graphics_editor.src.gui.gui import Gui
from src.python.graphics_editor.src.logic.image import Image


def main() -> None:
    image = Image(640, 480)
    gui = Gui(image)
    gui.run()


if __name__ == "__main__":
    main()
