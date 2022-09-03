import unittest

from src.game_logic.dice import Dice
from src.game_logic.dice_to_rule_mapper import DiceToRulesMapper
from src.game_logic.table import ScoreType


class TestDiceToRulesMapper(unittest.TestCase):
    def test_map_to_rules_mixed_1(self):
        dices = [Dice(1), Dice(2), Dice(3), Dice(4), Dice(5), Dice(6)]
        mapper = DiceToRulesMapper(dices)
        excepted_rules = [ScoreType.ACES, ScoreType.TWOS, ScoreType.THREES, ScoreType.FOURS, ScoreType.FIVES,
                          ScoreType.SIXES, ScoreType.SMALL_STRAIGHT, ScoreType.LARGE_STRAIGHT, ScoreType.CHANCE]
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))

    def test_map_to_rules_mixed_2(self):
        dices = [Dice(1), Dice(1), Dice(1), Dice(1), Dice(1), Dice(1)]
        mapper = DiceToRulesMapper(dices)
        excepted_rules = [ScoreType.ACES, ScoreType.THREE_OF_A_KIND, ScoreType.FOUR_OF_A_KIND, ScoreType.YAHTZEE, ScoreType.CHANCE]
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))

    def test_map_to_rules_mixed_3(self):
        dices = [Dice(1), Dice(1), Dice(1), Dice(2), Dice(2), Dice(2)]
        mapper = DiceToRulesMapper(dices)
        excepted_rules = [ScoreType.ACES, ScoreType.TWOS, ScoreType.THREE_OF_A_KIND, ScoreType.FULL_HOUSE, ScoreType.CHANCE]
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))

    def test_map_to_rules_empty(self):
        dices = [Dice(-1), Dice(-1), Dice(-1), Dice(-1), Dice(-1), Dice(-1)]
        mapper = DiceToRulesMapper(dices)
        excepted_rules = []
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))

    def test_map_to_rules_only_threes(self):
        dices = [Dice(3), Dice(3), Dice(3)]
        mapper = DiceToRulesMapper(dices)
        excepted_rules = [ScoreType.THREES, ScoreType.THREE_OF_A_KIND, ScoreType.CHANCE]
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))

