from typing import List

from src.game_logic.dice import Dice
from src.game_logic.table import ScoreType
from src.utils.utils import create_histogram


class DiceToRulesMapper:
    def __init__(self, dices: List[Dice]):
        self.dices = dices

    def map_to_rules(self) -> List[ScoreType]:
        for dice in self.dices:
            if dice.value == -1:
                return []

        def could_be_aces(dices: List[int]) -> bool:
            return any(dice.value == 1 for dice in dices)

        def could_be_twos(dices: List[int]) -> bool:
            return any(dice.value == 2 for dice in dices)

        def could_be_threes(dices: List[int]) -> bool:
            return any(dice.value == 3 for dice in dices)

        def could_be_fours(dices: List[int]) -> bool:
            return any(dice.value == 4 for dice in dices)

        def could_be_fives(dices: List[int]) -> bool:
            return any(dice.value == 5 for dice in dices)

        def could_be_sixes(dices: List[int]) -> bool:
            return any(dice.value == 6 for dice in dices)

        def could_be_three_of_a_kind(dices: List[int]) -> bool:
            histogram = create_histogram(dices)
            return max(histogram.values()) >= 3

        def could_be_four_of_a_kind(dices: List[int]) -> bool:
            histogram = create_histogram(dices)
            return max(histogram.values()) >= 4

        def could_be_full_house(dices: List[int]) -> bool:
            histogram = create_histogram(dices)
            are_two_same = False
            are_three_same = False
            for value, count in histogram.items():
                if count >= 3:
                    are_two_same = True
                elif count >= 2:
                    are_three_same = True
            return are_two_same and are_three_same

        def could_be_small_straight(dices: List[int]) -> bool:
            histogram = create_histogram(dices)
            return len(histogram) >= 5 and (
                    1 in histogram and 2 in histogram and 3 in histogram and 4 in histogram
            )

        def could_be_large_straight(dices: List[int]) -> bool:
            histogram = create_histogram(dices)
            return len(histogram) >= 5 and (
                    2 in histogram and 3 in histogram and 4 in histogram and 5 in histogram
            )

        def could_be_chance(_: List[int]) -> bool:
            return True

        def could_be_yahtzee(dices: List[int]) -> bool:
            histogram = create_histogram(dices)
            return max(histogram.values()) >= 5

        all_rules = [
            could_be_aces,
            could_be_twos,
            could_be_threes,
            could_be_fours,
            could_be_fives,
            could_be_sixes,
            could_be_three_of_a_kind,
            could_be_four_of_a_kind,
            could_be_full_house,
            could_be_small_straight,
            could_be_large_straight,
            could_be_chance,
            could_be_yahtzee,
        ]
        rules = []

        for rule in all_rules:
            if rule(self.dices):
                rules.append(self.map_to_score_type(rule))
        return rules

    def map_to_score_type(self, rule: callable) -> ScoreType:
        name = rule.__name__[len("could_be_"):].upper()
        return ScoreType[name]