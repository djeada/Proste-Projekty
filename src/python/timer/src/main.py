"""
Python implementation of a simple timer/stopwatch application.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.timer.src.gui.gui import Gui
from src.python.timer.src.logic.timer import Timer


def main() -> None:
    timer = Timer()
    gui = Gui(timer)
    gui.run()


if __name__ == "__main__":
    main()
