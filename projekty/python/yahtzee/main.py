import tkinter as tk
from dataclasses import dataclass
from enum import Enum, auto
import random
from typing import List, Dict


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

class Dice:
    def __init__(self, value: int = -1):
        self.value = value

    def roll(self) -> "Dice":
        self.value = random.randint(1, 6)
        return self

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return f"{self.value}"
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
            self.aces = sum([dice.value for dice in dices])
        elif score_type == ScoreType.TWOS:
            self.twos = sum([dice.value for dice in dices])
        elif score_type == ScoreType.THREES:
            self.trees = sum([dice.value for dice in dices])
        elif score_type == ScoreType.FOURS:
            self.fours = sum([dice.value for dice in dices])
        elif score_type == ScoreType.FIVES:
            self.fives = sum([dice.value for dice in dices])
        elif score_type == ScoreType.SIXES:
            self.sixes = sum([dice.value for dice in dices])
        elif score_type == ScoreType.THREE_OF_A_KIND:
            self.three_of_a_kind += sum([dice.value for dice in dices])
        elif score_type == ScoreType.FOUR_OF_A_KIND:
            self.four_of_a_kind += sum([dice.value for dice in dices])
        elif score_type == ScoreType.FULL_HOUSE:
            self.full_house += sum([dice.value for dice in dices])
        elif score_type == ScoreType.SMALL_STRAIGHT:
            if len(dices) >= 5:
                self.small_straight += 40
            elif len(dices) >= 4:
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





def create_histogram(dices: List[int]) -> Dict[int, int]:
    histogram = {}
    for dice in dices:
        if dice.value in histogram:
            histogram[dice.value] += 1
        else:
            histogram[dice.value] = 1
    return histogram


class DiceToRulesMapper:
    def __init__(self, dices: List[Dice]):
        self.dices = dices

    def map_to_rules(self) -> List[ScoreType]:
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


class Player(Enum):
    A = auto()
    B = auto()


class Game:
    def __init__(self):
        self.player_a_table: Table = Table()
        self.player_b_table: Table = Table()
        self.dices: List[Dice] = [Dice() for i in range(6)]
        self.dices_put_down: List[int] = []
        self.current_player: Player = Player.A
        self.numbers_of_throws_left: int = 3

    def roll_dices(self) -> None:
        self.dices = [
            self.dices[i].roll()
            for i in range(len(self.dices))
            if i not in self.dices_put_down
        ]
        self.numbers_of_throws_left -= 1
        if self.numbers_of_throws_left == 0:
            self.switch_player()

    def switch_player(self) -> None:
        if self.current_player == Player.A:
            self.current_player = Player.B
        else:
            self.current_player = Player.A

        self.numbers_of_throws_left = 3
        self.dices = []
        self.dices_put_down = []

    def filtered_dices(self):
        return [
            dice for i, dice in enumerate(self.dices) if i not in self.dices_put_down
        ]

    def valid_rules_for_current_player(self) -> List[ScoreType]:
        mapper = DiceToRulesMapper(self.filtered_dices())

        def filter_rules(rules: List[ScoreType], table: Table) -> List[ScoreType]:
            return [rule for rule in rules if not table.is_rule_already_used(rule)]

        return filter_rules(mapper.map_to_rules(), self.current_player_table())

    def current_player_table(self) -> Table:
        if self.current_player == Player.A:
            return self.player_a_table
        else:
            return self.player_b_table

    def put_down_dices(self, indices: List[int]) -> None:
        self.dices_put_down = indices

    def update_table_current_player(self, score_type: ScoreType) -> None:
        self.current_player_table().add_score(score_type, self.filtered_dices())


class TableGui:
    def __init__(self, frame: tk.Frame, table: Table):
        self.frame = frame
        self.table = table
        self.draw()

    def draw(self) -> None:
        # add grid with two columns each with label and score
        for i, score_type in enumerate(ScoreType):
            label = tk.Label(self.frame, text=score_type.name)
            label.grid(row=i, column=0)
            score = tk.Label(self.frame, text=self.table.get_score(score_type))
            score.grid(row=i, column=1, padx=10)

class DicesGui:
    def __init__(self, frame: tk.Frame, dices: List[Dice]):
        self.frame = frame
        self.dices = dices
        self.draw()

    def draw(self) -> None:
        # draw each dice as a button with text of value
        for i, dice in enumerate(self.dices):
            button = tk.Button(self.frame, text=dice.value)
            button.grid(row=0, column=i, padx=10)
        
class GameGui:
    def __init__(self, master: tk.Tk):
        self.master = master
        self.game = Game()
        # create four frames for the table and dices and put them in a grid
        # add Label for player A
        # add margin between frames and grid
        player_a_label = tk.Label(self.master, text="Player A")
        player_a_label.grid(row=0, column=0, padx=10, pady=10)

        self.table_a_frame = tk.Frame(self.master)
        self.table_a_frame.grid(row=1, column=0, padx=10)

        self.dices_a_frame = tk.Frame(self.master)
        self.dices_a_frame.grid(row=1, column=1, padx=10)

        # add Label for player B
        player_b_label = tk.Label(self.master, text="Player B")
        player_b_label.grid(row=2, column=0, padx=10, pady=10)

        self.table_b_frame = tk.Frame(self.master)
        self.table_b_frame.grid(row=3, column=0, padx=10)

        self.dices_b_frame = tk.Frame(self.master)
        self.dices_b_frame.grid(row=3, column=1, padx=10)

        self.setup()

    def setup(self) -> None:

        for frame, table in [(self.table_a_frame, self.game.player_a_table), (self.table_b_frame, self.game.player_b_table)]:
            table_gui = TableGui(frame, table)
        
        for frame, dices in [(self.dices_a_frame, self.game.dices), (self.dices_b_frame, self.game.dices)]:
            dices_gui = DicesGui(frame, dices)

    def draw_dices(self) -> None:
        self.dices_frame = tk.Frame(self.master)
        self.dices_frame.grid(row=0, column=0)
        self.dices_gui = DicesGui(self.dices_frame, self.game.dices)

    def draw_table(self) -> None:
        self.table_frame = tk.Frame(self.master)
        self.table_frame.grid(row=0, column=1)
        self.table_gui = TableGui(self.table_frame, self.game.current_player_table())

    def draw_buttons(self) -> None:
        self.buttons_frame = tk.Frame(self.master)
        self.buttons_frame.grid(row=1, column=0)
        self.roll_button = tk.Button(self.buttons_frame, text="Roll", command=self.roll)
        self.roll_button.grid(row=0, column=0)
        self.put_down_button = tk.Button(self.buttons_frame, text="Put down", command=self.put_down)
        self.put_down_button.grid(row=0, column=1)
        self.rules_button = tk.Button(self.buttons_frame, text="Rules", command=self.rules)
        self.rules_button.grid(row=0, column=2)
        self.rules_button = tk.Button(self.buttons_frame, text="Rules", command=self.rules)
        self.rules_button.grid(row=0, column=3)
        self.rules_button = tk.Button(self.buttons_frame, text="Rules", command=self.rules)
        self.rules_button.grid(row=0, column=4)

    def roll(self) -> None:
        self.game.roll_dices()
        self.dices_gui.draw()
        self.table_gui.draw()

    def put_down(self) -> None:
        self.game.put_down_dices(self.dices_gui.dices_put_down)
        self.dices_gui.draw()
        self.table_gui.draw()

    def rules(self) -> None:
        rules = self.game.valid_rules_for_current_player()
        print(rules)


def main():
    root = tk.Tk()
    game_gui = GameGui(root)
    root.mainloop()

if __name__ == "__main__":
    main()
