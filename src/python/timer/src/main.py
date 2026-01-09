"""
Python implementation of a simple timer/stopwatch application.
"""
from src.python.timer.src.gui.gui import Gui
from src.python.timer.src.logic.timer import Timer


def main() -> None:
    timer = Timer()
    gui = Gui(timer)
    gui.run()


if __name__ == "__main__":
    main()
