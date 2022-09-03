from dataclasses import dataclass
from enum import auto, Enum
from typing import List

from src.game_logic.dice import Dice
from src.utils.utils import most_common_value


class ScoreType(Enum):
    ACES = auto()
    TWOS = auto()
    THREES = auto()
    FOURS = auto()
    FIVES = auto()
    SIXES = auto()
    THREE_OF_A_KIND = auto()
    FOUR_OF_A_KIND = auto()
    FULL_HOUSE = auto()
    SMALL_STRAIGHT = auto()
    LARGE_STRAIGHT = auto()
    CHANCE = auto()
    YAHTZEE = auto()

@dataclass
class Table:
    aces: int = 0
    twos: int = 0
    threes: int = 0
    fours: int = 0
    fives: int = 0
    sixes: int = 0
    three_of_a_kind: int = 0
    four_of_a_kind: int = 0
    full_house: int = 0
    small_straight: int = 0
    large_straight: int = 0
    chance: int = 0
    yahtzee: int = 0
    bonus: int = 0

    def total(self):
        upper_section_total = (
                self.aces + self.twos + self.threes + self.fours + self.fives + self.sixes
        )
        if upper_section_total >= 63:
            self.bonus += 35

        lower_section_total = (
                self.three_of_a_kind
                + self.four_of_a_kind
                + self.full_house
                + self.small_straight
                + self.large_straight
                + self.chance
                + self.yahtzee
        )
        return upper_section_total + lower_section_total + self.bonus

    def add_score(self, score_type: ScoreType, dices: List[Dice]) -> None:
        if score_type == ScoreType.ACES:
            self.aces = sum([dice.value for dice in dices if dice.value == 1])
        elif score_type == ScoreType.TWOS:
            self.twos = sum([dice.value for dice in dices if dice.value == 2])
        elif score_type == ScoreType.THREES:
            self.threes = sum([dice.value for dice in dices if dice.value == 3])
        elif score_type == ScoreType.FOURS:
            self.fours = sum([dice.value for dice in dices if dice.value == 4])
        elif score_type == ScoreType.FIVES:
            self.fives = sum([dice.value for dice in dices if dice.value == 5])
        elif score_type == ScoreType.SIXES:
            self.sixes = sum([dice.value for dice in dices if dice.value == 6])
        elif score_type == ScoreType.THREE_OF_A_KIND:
            self.three_of_a_kind += sum([dice.value for dice in dices if dice.value == most_common_value(dices)])
        elif score_type == ScoreType.FOUR_OF_A_KIND:
            self.four_of_a_kind += sum([dice.value for dice in dices if dice.value == most_common_value(dices)])
        elif score_type == ScoreType.FULL_HOUSE:
            self.full_house += 25
        elif score_type == ScoreType.SMALL_STRAIGHT:
            self.small_straight += 30
        elif score_type == ScoreType.LARGE_STRAIGHT:
            self.large_straight += 40
        elif score_type == ScoreType.CHANCE:
            self.chance += sum([dice.value for dice in dices])
        elif score_type == ScoreType.YAHTZEE:
            self.yahtzee += 50

    def is_rule_already_used(self, score_type: ScoreType) -> bool:
        return self.get_score(score_type) != 0

    def get_score(self, score_type: ScoreType) -> int:
        if score_type == ScoreType.ACES:
            return self.aces
        elif score_type == ScoreType.TWOS:
            return self.twos
        elif score_type == ScoreType.THREES:
            return self.threes
        elif score_type == ScoreType.FOURS:
            return self.fours
        elif score_type == ScoreType.FIVES:
            return self.fives
        elif score_type == ScoreType.SIXES:
            return self.sixes
        elif score_type == ScoreType.THREE_OF_A_KIND:
            return self.three_of_a_kind
        elif score_type == ScoreType.FOUR_OF_A_KIND:
            return self.four_of_a_kind
        elif score_type == ScoreType.FULL_HOUSE:
            return self.full_house
        elif score_type == ScoreType.SMALL_STRAIGHT:
            return self.small_straight
        elif score_type == ScoreType.LARGE_STRAIGHT:
            return self.large_straight
        elif score_type == ScoreType.CHANCE:
            return self.chance
        elif score_type == ScoreType.YAHTZEE:
            return self.yahtzee

        return -1