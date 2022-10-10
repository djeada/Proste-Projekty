from dataclasses import field, dataclass
from enum import Enum, auto
from typing import List

from src.python.yahtzee.src.logic.dice import Dice
from src.python.yahtzee.src.logic.dice_to_rule_mapper import DiceToRulesMapper
from src.python.yahtzee.src.logic.table import Table, ScoreType


class PlayerType(Enum):
    """
    Possible types of players.
    """

    A = auto()
    B = auto()


@dataclass
class Player:
    """
    Model of a player in the YAHTZE game.
    """

    player_type: PlayerType
    table: Table = field(default_factory=lambda: Table())
    dice_list: List[Dice] = field(default_factory=lambda: [Dice() for _ in range(6)])
    dice_list_put_away: List[int] = field(default_factory=list)
    numbers_of_throws_left: int = 3

    def roll_dice(self) -> None:
        """
        Rolls the dice that are held by the player.
        """
        for dice in self.kept_dice_list():
            dice.roll()

        self.numbers_of_throws_left -= 1

    def kept_dice_list(self) -> List[Dice]:
        """
        Gets the list of dice that are kept by the player.

        :return: list of dice that are kept by the player
        """
        return [
            dice
            for i, dice in enumerate(self.dice_list)
            if i not in self.dice_list_put_away
        ]

    def valid_rules(self) -> List[ScoreType]:
        """
        Checks which rules are valid for the current dice list.

        :return: list of valid rules
        """
        mapper = DiceToRulesMapper(self.kept_dice_list())

        def filter_rules(rules: List[ScoreType], table: Table) -> List[ScoreType]:
            return [rule for rule in rules if not table.is_rule_already_used(rule)]

        return filter_rules(mapper.map_to_rules(), self.table)

    def update_table(self, score_type: ScoreType) -> None:
        """
        Updates the table of the player.

        :param score_type: score type to update
        """
        self.table.add_score(score_type, self.kept_dice_list())
        self.reset()
        self.numbers_of_throws_left = 0

    def reset(self) -> None:
        """
        Resets the player state.
        """
        for dice in self.dice_list:
            dice.invalidate()
        self.dice_list_put_away.clear()
        self.numbers_of_throws_left = 3
