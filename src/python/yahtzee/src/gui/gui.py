from tkinter import ttk

from src.python.yahtzee.src.gui.dice_gui import DiceGui
from src.python.yahtzee.src.gui.possible_moves_gui import PossibleMovesGui
from src.python.yahtzee.src.gui.sytled_window import StyledWindow
from src.python.yahtzee.src.gui.table_gui import TableGui
from src.python.yahtzee.src.logic.game import Game


class Gui:
    """
    Application window.
    Holds all the GUI elements.
    """

    def __init__(self):
        self.root = StyledWindow()
        self.root.title("Yahtzee")
        self.root.geometry("1200x900")

        self.game = Game()
        self.table_guis = []
        self.dice_list_guis = []
        self.possible_moves_guis = []
        # create four frames for the table and dice_list and put them in a grid
        # add Label for player A
        # add margin between frames and grid
        player_a_label = ttk.Label(self.root, text="Player A")
        player_a_label.grid(row=0, column=0, padx=10, pady=10)

        self.table_a_frame = ttk.Frame(self.root)
        self.table_a_frame.grid(row=1, column=0, padx=10)

        self.dice_list_a_frame = ttk.Frame(self.root, height=300, width=800)
        self.dice_list_a_frame.grid_propagate(0)
        self.dice_list_a_frame.grid(row=1, column=1, padx=10)

        self.possible_moves_a_frame = ttk.Frame(self.root)
        self.possible_moves_a_frame.grid(row=1, column=2, padx=10)

        # add Label for player B
        player_b_label = ttk.Label(self.root, text="Player B")
        player_b_label.grid(row=2, column=0, padx=10, pady=10)

        self.table_b_frame = ttk.Frame(self.root)
        self.table_b_frame.grid(row=3, column=0, padx=10)

        self.dice_list_b_frame = ttk.Frame(self.root, height=300, width=800)
        self.dice_list_b_frame.grid_propagate(0)
        self.dice_list_b_frame.grid(row=3, column=1, padx=10)

        self.possible_moves_b_frame = ttk.Frame(self.root)
        self.possible_moves_b_frame.grid(row=3, column=2, padx=10)

        self.setup()
        self.draw()

    def setup(self) -> None:
        """
        Sets up the GUI.
        """

        for frame, table in [
            (self.table_a_frame, self.game.player_a.table),
            (self.table_b_frame, self.game.player_b.table),
        ]:
            self.table_guis.append(TableGui(frame, table))

        for frame, dice_list, dice_list_put_away in [
            (
                self.dice_list_a_frame,
                self.game.player_a.dice_list,
                self.game.player_a.dice_list_put_away,
            ),
            (
                self.dice_list_b_frame,
                self.game.player_b.dice_list,
                self.game.player_b.dice_list_put_away,
            ),
        ]:
            self.dice_list_guis.append(
                DiceGui(frame, dice_list, dice_list_put_away, self)
            )

        for frame, player in [
            (self.possible_moves_a_frame, self.game.player_a),
            (self.possible_moves_b_frame, self.game.player_b),
        ]:
            self.possible_moves_guis.append(
                PossibleMovesGui(frame, player, self.game, self)
            )

    def draw(self) -> None:
        """
        Draws the GUI.
        """
        for gui_element in (
            self.table_guis + self.dice_list_guis + self.possible_moves_guis
        ):
            gui_element.draw()

    def roll_dice(self) -> None:
        """
        Rolls the dice.
        """
        self.game.roll_dice()
        self.draw()

    def run(self) -> None:
        """
        Starts the main loop of the GUI.
        """
        self.root.mainloop()
