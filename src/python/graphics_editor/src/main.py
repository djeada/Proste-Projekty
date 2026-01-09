"""
Python implementation of a simple graphics editor.
"""
from src.python.graphics_editor.src.gui.gui import Gui
from src.python.graphics_editor.src.logic.image import Image


def main() -> None:
    image = Image(640, 480)
    gui = Gui(image)
    gui.run()


if __name__ == "__main__":
    main()
