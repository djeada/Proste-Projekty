"""Rules of hangman: choosing a word, applying guesses and the game state."""

import random
import string
from enum import Enum
from typing import List, NamedTuple, Optional

MAX_MISSES = 6


class Entry(NamedTuple):
    category: str
    word: str


WORD_LIST: List[Entry] = [
    Entry("Animal", "elephant"), Entry("Animal", "giraffe"), Entry("Animal", "penguin"),
    Entry("Animal", "kangaroo"), Entry("Animal", "dolphin"), Entry("Fruit", "banana"),
    Entry("Fruit", "cherry"), Entry("Fruit", "orange"), Entry("Fruit", "mango"),
    Entry("Fruit", "lemon"), Entry("Country", "canada"), Entry("Country", "norway"),
    Entry("Country", "brazil"), Entry("Country", "japan"), Entry("Country", "egypt"),
    Entry("Coding", "python"), Entry("Coding", "compiler"), Entry("Coding", "function"),
    Entry("Coding", "keyboard"), Entry("Coding", "variable"),
]


class GuessResult(Enum):
    HIT = "hit"
    MISS = "miss"
    REPEAT = "repeat"
    INVALID = "invalid"


class GameStatus(Enum):
    PLAYING = "playing"
    WON = "won"
    LOST = "lost"


def random_entry(rng: Optional[random.Random] = None) -> Entry:
    """Picks a word; pass a seeded random.Random to make the choice repeatable."""
    rng = rng or random.Random()
    return rng.choice(WORD_LIST)


class Game:
    def __init__(self, entry: Entry):
        self.category = entry.category
        self.word = entry.word
        self.guessed = set()
        self.misses = 0

    def guess(self, letter: str) -> GuessResult:
        letter = letter.lower()
        if len(letter) != 1 or letter not in string.ascii_lowercase:
            return GuessResult.INVALID
        if letter in self.guessed:
            return GuessResult.REPEAT
        self.guessed.add(letter)
        if letter in self.word:
            return GuessResult.HIT
        self.misses += 1
        return GuessResult.MISS

    def masked(self) -> str:
        """The word with unknown letters as underscores, e.g. "_ a _ a _ a"."""
        return " ".join(letter if letter in self.guessed else "_" for letter in self.word)

    @property
    def status(self) -> GameStatus:
        if self.misses >= MAX_MISSES:
            return GameStatus.LOST
        if all(letter in self.guessed for letter in self.word):
            return GameStatus.WON
        return GameStatus.PLAYING
