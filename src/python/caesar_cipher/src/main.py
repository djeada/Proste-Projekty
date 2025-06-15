"""
A desktop application that uses the Caesar cipher 
algorithm in the background to cipher or decipher 
text messages.
"""

from src.gui.gui import Gui


def main():
    gui = Gui()
    gui.run()


if __name__ == "__main__":
    main()
