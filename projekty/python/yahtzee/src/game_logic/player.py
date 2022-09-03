from dataclasses import field, dataclass
from enum import Enum, auto
from typing import List

from src.game_logic.dice import Dice
from src.game_logic.dice_to_rule_mapper import DiceToRulesMapper
from src.game_logic.table import ScoreType, Table


class PlayerType(Enum):
    A = auto()
    B = auto()


@dataclass
class Player:
    player_type: PlayerType
    table: Table = field(default_factory=lambda:Table())
    dices: List[Dice] = field(default_factory=lambda: [Dice() for _ in range(6)])
    dices_put_away: List[int] = field(default_factory=list)
    numbers_of_throws_left: int = 3

    def roll_dices(self):
        for dice in self.kept_dices():
            dice.roll()

        self.numbers_of_throws_left -= 1

    def kept_dices(self) -> List[Dice]:
        return [dice for i, dice in enumerate(self.dices) if i not in self.dices_put_away]

    def valid_rules(self) -> List[ScoreType]:
        mapper = DiceToRulesMapper(self.kept_dices())

        def filter_rules(rules: List[ScoreType], table: Table) -> List[ScoreType]:
            return [rule for rule in rules if not table.is_rule_already_used(rule)]

        return filter_rules(mapper.map_to_rules(), self.table)

    def update_table(self, score_type: ScoreType) -> None:
        self.table.add_score(score_type, self.kept_dices())
        self.reset()
        self.numbers_of_throws_left = 0

    def reset(self):
        for dice in self.dices:
            dice.invalidate()
        self.dices_put_away.clear()
        self.numbers_of_throws_left = 3
