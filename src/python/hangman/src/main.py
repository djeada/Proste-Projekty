"""
Game of hangman.
1. The word is chosen randomly from the list of words.
2. The player has to guess the word.
3. There are n squares drawn on the board, where n is the length of the word.
4. When the player guesses a letter, the squares with the letter are revealed.
5. If the player guesses the word, he wins.
6. If the player guesses a letter that is not in the word, an image from a list
of images is drawn.

The board consists of an image on the left side and the squares representing the
letters on the right side.
"""

from src.python.hangman.src.gui.gui import Gui
from src.python.hangman.src.logic.config import Config
from src.python.hangman.src.utils.consts import Consts


def main() -> None:
    config = Config(Consts.paths, Consts.words)
    gui = Gui(config)
    gui.run()


if __name__ == "__main__":
    main()
