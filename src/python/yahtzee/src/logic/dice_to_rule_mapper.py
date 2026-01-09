from typing import List

try:
    from dice import Dice
    from table import ScoreType
    from utils import create_histogram
except ImportError:
    from src.logic.dice import Dice
    from src.logic.table import ScoreType
    from src.utils.utils import create_histogram


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

        def could_be_aces(dice_list: List[Dice]) -> bool:
            return any(dice.value == 1 for dice in dice_list)

        def could_be_twos(dice_list: List[Dice]) -> bool:
            return any(dice.value == 2 for dice in dice_list)

        def could_be_threes(dice_list: List[Dice]) -> bool:
            return any(dice.value == 3 for dice in dice_list)

        def could_be_fours(dice_list: List[Dice]) -> bool:
            return any(dice.value == 4 for dice in dice_list)

        def could_be_fives(dice_list: List[Dice]) -> bool:
            return any(dice.value == 5 for dice in dice_list)

        def could_be_sixes(dice_list: List[Dice]) -> bool:
            return any(dice.value == 6 for dice in dice_list)

        def could_be_three_of_a_kind(dice_list: List[Dice]) -> bool:
            histogram = create_histogram(dice_list)
            return max(histogram.values()) >= 3

        def could_be_four_of_a_kind(dice_list: List[Dice]) -> bool:
            histogram = create_histogram(dice_list)
            return max(histogram.values()) >= 4

        def could_be_full_house(dice_list: List[Dice]) -> bool:
            histogram = create_histogram(dice_list)
            are_two_same = False
            are_three_same = False
            for value, count in histogram.items():
                if count >= 3 and not are_three_same:
                    are_three_same = True
                elif count >= 2:
                    are_two_same = True
            return are_two_same and are_three_same

        def could_be_small_straight(dice_list: List[Dice]) -> bool:
            histogram = create_histogram(dice_list)
            # Small straight: any 4 sequential dice (1-2-3-4, 2-3-4-5, or 3-4-5-6)
            small_straights = [
                {1, 2, 3, 4},
                {2, 3, 4, 5},
                {3, 4, 5, 6},
            ]
            values = set(histogram.keys())
            return any(straight.issubset(values) for straight in small_straights)

        def could_be_large_straight(dice_list: List[Dice]) -> bool:
            histogram = create_histogram(dice_list)
            # Large straight: 5 sequential dice (1-2-3-4-5 or 2-3-4-5-6)
            large_straights = [
                {1, 2, 3, 4, 5},
                {2, 3, 4, 5, 6},
            ]
            values = set(histogram.keys())
            return any(straight.issubset(values) for straight in large_straights)

        def could_be_chance(_: List[Dice]) -> bool:
            return True

        def could_be_yahtzee(dice_list: List[Dice]) -> bool:
            histogram = create_histogram(dice_list)
            # Yahtzee requires 5 or more dice of the same value
            return len(dice_list) >= 5 and max(histogram.values()) >= 5

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
