import tkinter as tk

from src.game_logic.game import Game
from src.gui.dice_gui import DiceGui
from src.gui.possible_moves_gui import PossibleMovesGui
from src.gui.table_gui import TableGui


class GameGui:
    def __init__(self, master: tk.Tk):
        self.master = master
        self.game = Game()
        self.table_guis = []
        self.dice_list_guis = []
        self.possible_moves_guis = []
        # create four frames for the table and dice_list and put them in a grid
        # add Label for player A
        # add margin between frames and grid
        player_a_label = tk.Label(self.master, text="Player A")
        player_a_label.grid(row=0, column=0, padx=10, pady=10)

        self.table_a_frame = tk.Frame(self.master)
        self.table_a_frame.grid(row=1, column=0, padx=10)

        self.dice_list_a_frame = tk.Frame(self.master,  height=300,width=330)
        self.dice_list_a_frame.grid_propagate(0)
        self.dice_list_a_frame.grid(row=1, column=1, padx=10)

        self.possible_moves_a_frame = tk.Frame(self.master)
        self.possible_moves_a_frame.grid(row=1, column=2, padx=10)

        # add Label for player B
        player_b_label = tk.Label(self.master, text="Player B")
        player_b_label.grid(row=2, column=0, padx=10, pady=10)

        self.table_b_frame = tk.Frame(self.master)
        self.table_b_frame.grid(row=3, column=0, padx=10)

        self.dice_list_b_frame = tk.Frame(self.master,  height=300,width=330)
        self.dice_list_b_frame.grid_propagate(0)
        self.dice_list_b_frame.grid(row=3, column=1, padx=10)

        self.possible_moves_b_frame = tk.Frame(self.master)
        self.possible_moves_b_frame.grid(row=3, column=2, padx=10)

        self.setup()
        self.draw()

    def setup(self) -> None:

        for frame, table in [(self.table_a_frame, self.game.player_a.table),
                             (self.table_b_frame, self.game.player_b.table)]:
            self.table_guis.append(TableGui(frame, table))

        for frame, dice_list, dice_list_put_away in [(self.dice_list_a_frame, self.game.player_a.dice_list, self.game.player_a.dice_list_put_away),
        (self.dice_list_b_frame, self.game.player_b.dice_list, self.game.player_b.dice_list_put_away)]:
            self.dice_list_guis.append(DiceGui(frame, dice_list, dice_list_put_away, self))

        for frame, player in [(self.possible_moves_a_frame, self.game.player_a), (self.possible_moves_b_frame, self.game.player_b)]:
            self.possible_moves_guis.append(PossibleMovesGui(frame, player, self.game, self))

    def draw(self) -> None:
        for gui_element in self.table_guis + self.dice_list_guis + self.possible_moves_guis:
            gui_element.draw()

    def roll_dice(self) -> None:
        self.game.roll_dice()
        self.draw()