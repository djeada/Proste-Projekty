import unittest
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/logic')))
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/utils')))
from dice import Dice
from dice_to_rule_mapper import DiceToRulesMapper
from table import ScoreType


class TestDiceToRulesMapper(unittest.TestCase):
    def test_map_to_rules_mixed_1(self):
        dice_list = [Dice(1), Dice(2), Dice(3), Dice(4), Dice(5), Dice(6)]
        mapper = DiceToRulesMapper(dice_list)
        excepted_rules = [
            ScoreType.ACES,
            ScoreType.TWOS,
            ScoreType.THREES,
            ScoreType.FOURS,
            ScoreType.FIVES,
            ScoreType.SIXES,
            ScoreType.SMALL_STRAIGHT,
            ScoreType.LARGE_STRAIGHT,
            ScoreType.CHANCE,
        ]
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))

    def test_map_to_rules_mixed_2(self):
        dice_list = [Dice(1), Dice(1), Dice(1), Dice(1), Dice(1), Dice(1)]
        mapper = DiceToRulesMapper(dice_list)
        excepted_rules = [
            ScoreType.ACES,
            ScoreType.THREE_OF_A_KIND,
            ScoreType.FOUR_OF_A_KIND,
            ScoreType.YAHTZEE,
            ScoreType.CHANCE,
        ]
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))

    def test_map_to_rules_mixed_3(self):
        dice_list = [Dice(1), Dice(1), Dice(1), Dice(2), Dice(2), Dice(2)]
        mapper = DiceToRulesMapper(dice_list)
        excepted_rules = [
            ScoreType.ACES,
            ScoreType.TWOS,
            ScoreType.THREE_OF_A_KIND,
            ScoreType.FULL_HOUSE,
            ScoreType.CHANCE,
        ]
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))

    def test_map_to_rules_empty(self):
        dice_list = [Dice(-1), Dice(-1), Dice(-1), Dice(-1), Dice(-1), Dice(-1)]
        mapper = DiceToRulesMapper(dice_list)
        excepted_rules = []
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))

    def test_map_to_rules_only_threes(self):
        dice_list = [Dice(3), Dice(3), Dice(3)]
        mapper = DiceToRulesMapper(dice_list)
        excepted_rules = [ScoreType.THREES, ScoreType.THREE_OF_A_KIND, ScoreType.CHANCE]
        result = mapper.map_to_rules()
        self.assertSetEqual(set(excepted_rules), set(result))
