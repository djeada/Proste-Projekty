from msilib.sequence import tables
import unittest

from src.game_logic.dice import Dice
from src.game_logic.table import Table, ScoreType


class TestTable(unittest.TestCase):
    def test_add_score_threes(self):
        table = Table()
        dice_list = [Dice(3), Dice(3), Dice(3)]
        score_type = ScoreType.THREES
        table.add_score(score_type, dice_list)
        excepted_score = 9
        result = table.get_score(score_type)
        self.assertEqual(excepted_score, result)

        table = Table()
        dice_list = [Dice(3), Dice(3), Dice(3), Dice(1), Dice(2), Dice(5)]
        table.add_score(score_type, dice_list)
        result = table.get_score(score_type)
        self.assertEqual(excepted_score, result)

    def test_add_score_small_straight(self):
        table = Table()
        dice_list = [Dice(1), Dice(2), Dice(3), Dice(4), Dice(1)]
        score_type = ScoreType.SMALL_STRAIGHT
        table.add_score(score_type, dice_list)
        excepted_score = 30
        result = table.get_score(score_type)
        self.assertEqual(excepted_score, result)

    def test_add_score_large_straight(self):
        table = Table()
        dice_list = [Dice(2), Dice(3), Dice(4), Dice(5), Dice(6)]
        score_type = ScoreType.LARGE_STRAIGHT
        table.add_score(score_type, dice_list)
        excepted_score = 40
        result = table.get_score(score_type)
        self.assertEqual(excepted_score, result)

    def test_add_score_full_house(self):
        table = Table()
        dice_list = [Dice(1), Dice(1), Dice(1), Dice(2), Dice(2)]
        score_type = ScoreType.FULL_HOUSE
        table.add_score(score_type, dice_list)
        excepted_score = 25
        result = table.get_score(score_type)
        self.assertEqual(excepted_score, result)

    def test_add_score_yahtzee(self):
        table = Table()
        dice_list = [Dice(1), Dice(1), Dice(1), Dice(1), Dice(1)]
        score_type = ScoreType.YAHTZEE
        table.add_score(score_type, dice_list)
        excepted_score = 50
        result = table.get_score(score_type)
        self.assertEqual(excepted_score, result)
