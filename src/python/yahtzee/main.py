"""
Desktop version of the popular dice game Yahtzee. 
The game is played by rolling five dice and scoring 
the roll in one of thirteen categories. 
The player with the highest score wins.
"""
from src.gui.gui import Gui


def main():
    gui = Gui()
    gui.run()


if __name__ == "__main__":
    main()
