from typing import List

from src.python.yahtzee.src.logic.dice import Dice
from src.python.yahtzee.src.logic.table import ScoreType
from src.python.yahtzee.src.utils.utils import create_histogram


class DiceToRulesMapper:
    """
    Defines the mapping between the dice and the rules.
    """

    def __init__(self, dice_list: List[Dice]):
        self.dice_list = dice_list

    def map_to_rules(self) -> List[ScoreType]:
        """
        Checks which rules are satisfied by the dice.

        :return: list of rules satisfied by the dice
        """
        for dice in self.dice_list:
            if dice.value == -1:
                return []

        def could_be_aces(dice_list: List[int]) -> bool:
            return any(dice.value == 1 for dice in dice_list)

        def could_be_twos(dice_list: List[int]) -> bool:
            return any(dice.value == 2 for dice in dice_list)

        def could_be_threes(dice_list: List[int]) -> bool:
            return any(dice.value == 3 for dice in dice_list)

        def could_be_fours(dice_list: List[int]) -> bool:
            return any(dice.value == 4 for dice in dice_list)

        def could_be_fives(dice_list: List[int]) -> bool:
            return any(dice.value == 5 for dice in dice_list)

        def could_be_sixes(dice_list: List[int]) -> bool:
            return any(dice.value == 6 for dice in dice_list)

        def could_be_three_of_a_kind(dice_list: List[int]) -> bool:
            histogram = create_histogram(dice_list)
            return max(histogram.values()) >= 3

        def could_be_four_of_a_kind(dice_list: List[int]) -> bool:
            histogram = create_histogram(dice_list)
            return max(histogram.values()) >= 4

        def could_be_full_house(dice_list: List[int]) -> bool:
            histogram = create_histogram(dice_list)
            are_two_same = False
            are_three_same = False
            for value, count in histogram.items():
                if count >= 3 and not are_three_same:
                    are_three_same = True
                elif count >= 2:
                    are_two_same = True
            return are_two_same and are_three_same

        def could_be_small_straight(dice_list: List[int]) -> bool:
            histogram = create_histogram(dice_list)
            return len(histogram) >= 5 and (
                1 in histogram and 2 in histogram and 3 in histogram and 4 in histogram
            )

        def could_be_large_straight(dice_list: List[int]) -> bool:
            histogram = create_histogram(dice_list)
            return len(histogram) >= 5 and (
                2 in histogram and 3 in histogram and 4 in histogram and 5 in histogram
            )

        def could_be_chance(_: List[int]) -> bool:
            return True

        def could_be_yahtzee(dice_list: List[int]) -> bool:
            histogram = create_histogram(dice_list)
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
            if rule(self.dice_list):
                rules.append(self.map_to_score_type(rule))
        return rules

    def map_to_score_type(self, rule: callable) -> ScoreType:
        """
        Maps the rule to the score type.

        :param rule: rule to map
        :return: score type
        """
        name = rule.__name__[len("could_be_") :].upper()
        return ScoreType[name]
