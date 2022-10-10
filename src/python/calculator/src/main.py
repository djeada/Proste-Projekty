"""
A simple calculator that can perform basic 
arithmetic operations. The user can enter numbers 
and operators, and the result will be displayed 
if the expression is valid.
"""
from src.python.calculator.src.gui.gui import Gui


def main() -> None:
    gui = Gui()
    gui.run()


if __name__ == "__main__":
    main()
