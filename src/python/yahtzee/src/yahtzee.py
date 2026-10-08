"""Rules of Yahtzee: dice, scoring, upper bonus, turns and rounds. No input or output here."""

from enum import IntEnum
from typing import Callable, List, Optional

NUM_DICE = 5
MAX_ROLLS = 3
NUM_ROUNDS = 13
UPPER_BONUS_LIMIT = 63
UPPER_BONUS = 35


class Category(IntEnum):
    ONES = 0
    TWOS = 1
    THREES = 2
    FOURS = 3
    FIVES = 4
    SIXES = 5
    THREE_OF_A_KIND = 6
    FOUR_OF_A_KIND = 7
    FULL_HOUSE = 8
    SMALL_STRAIGHT = 9
    LARGE_STRAIGHT = 10
    YAHTZEE = 11
    CHANCE = 12


CATEGORY_NAMES = [
    "Ones",
    "Twos",
    "Threes",
    "Fours",
    "Fives",
    "Sixes",
    "Three of a Kind",
    "Four of a Kind",
    "Full House",
    "Small Straight",
    "Large Straight",
    "Yahtzee",
    "Chance",
]

Card = List[Optional[int]]  # one score per category, None while the category is unused


def score_for(dice: List[int], category: Category) -> int:
    counts = [dice.count(face) for face in range(7)]  # counts[face] for faces 1 to 6
    total = sum(dice)
    if category <= Category.SIXES:
        face = category + 1
        return face * counts[face]
    if category == Category.THREE_OF_A_KIND:
        return total if max(counts) >= 3 else 0
    if category == Category.FOUR_OF_A_KIND:
        return total if max(counts) >= 4 else 0
    if category == Category.FULL_HOUSE:
        return 25 if 3 in counts and 2 in counts else 0
    if category == Category.SMALL_STRAIGHT:
        return 30 if any(set(range(start, start + 4)) <= set(dice) for start in (1, 2, 3)) else 0
    if category == Category.LARGE_STRAIGHT:
        return 40 if any(set(range(start, start + 5)) <= set(dice) for start in (1, 2)) else 0
    if category == Category.YAHTZEE:
        return 50 if max(counts) == NUM_DICE else 0
    return total


def upper_total(card: Card) -> int:
    return sum(score for score in card[:Category.SIXES + 1] if score is not None)


def upper_bonus(card: Card) -> int:
    return UPPER_BONUS if upper_total(card) >= UPPER_BONUS_LIMIT else 0


def card_total(card: Card) -> int:
    lower = sum(score for score in card[Category.THREE_OF_A_KIND:] if score is not None)
    return upper_total(card) + upper_bonus(card) + lower


class Game:
    """A game for 1 to 4 players. Each round every player takes one turn."""

    def __init__(self, num_players: int):
        self.cards: List[Card] = [[None] * len(Category) for _ in range(num_players)]
        self.current = 0  # index of the player whose turn it is
        self.round = 1  # NUM_ROUNDS + 1 when the game is over
        self.dice = [0] * NUM_DICE
        self.held = [False] * NUM_DICE
        self.rolls = 0  # rolls made in this turn

    @property
    def is_over(self) -> bool:
        return self.round > NUM_ROUNDS

    def roll(self, face: Callable[[], int]) -> bool:
        """Roll the dice that are not held. face() returns one value from 1 to 6."""
        if self.is_over or self.rolls >= MAX_ROLLS:
            return False
        self.dice = [self.dice[i] if self.held[i] else face() for i in range(NUM_DICE)]
        self.rolls += 1
        return True

    def toggle_hold(self, die: int) -> bool:
        """Hold or release die 0 to 4. Needs at least one roll in this turn."""
        if self.is_over or self.rolls == 0:
            return False
        self.held[die] = not self.held[die]
        return True

    def choose(self, category: Category) -> bool:
        card = self.cards[self.current]
        if self.is_over or self.rolls == 0 or card[category] is not None:
            return False
        card[category] = score_for(self.dice, category)
        self._next_turn()
        return True

    def winner(self) -> int:
        """Index of the player with the highest total. Ties go to the player who comes first."""
        return max(range(len(self.cards)), key=lambda player: card_total(self.cards[player]))

    def _next_turn(self) -> None:
        self.dice = [0] * NUM_DICE
        self.held = [False] * NUM_DICE
        self.rolls = 0
        self.current += 1
        if self.current == len(self.cards):
            self.current = 0
            self.round += 1
