from tkinter import ttk

try:
    from gui.dice_gui import DiceGui
    from gui.possible_moves_gui import PossibleMovesGui
    from gui.styled_window import StyledWindow
    from gui.table_gui import TableGui
    from logic.game import Game
except ImportError:
    from src.gui.dice_gui import DiceGui
    from src.gui.possible_moves_gui import PossibleMovesGui
    from src.gui.styled_window import StyledWindow
    from src.gui.table_gui import TableGui
    from src.logic.game import Game


class Gui:
    """
    Application window.
    Holds all the GUI elements.
    """

    def __init__(self):
        self.root = StyledWindow()
        self.root.title("🎲 Yahtzee")
        self.root.geometry("1000x700")
        self.root.minsize(800, 600)

        self.game = Game()
        self.table_guis = []
        self.dice_list_guis = []
        self.possible_moves_guis = []

        # Configure grid weights for responsiveness
        self.root.grid_columnconfigure(0, weight=1)
        self.root.grid_columnconfigure(1, weight=2)
        self.root.grid_columnconfigure(2, weight=1)
        self.root.grid_rowconfigure(1, weight=1)
        self.root.grid_rowconfigure(3, weight=1)

        # Title
        title_label = ttk.Label(
            self.root,
            text="🎲 YAHTZEE 🎲",
            style="Header.TLabel",
            font=("Helvetica", 24, "bold")
        )
        title_label.grid(row=0, column=0, columnspan=3, pady=(20, 10))

        # Player A section
        self.player_a_label = ttk.Label(
            self.root,
            text="▶ Player A (Your Turn)",
            style="Active.TLabel"
        )
        self.player_a_label.grid(row=1, column=0, padx=20, pady=10, sticky="n")

        # Player A card frame
        self.player_a_card = ttk.Frame(self.root, style="Card.TFrame", padding=15)
        self.player_a_card.grid(row=1, column=0, padx=20, pady=(40, 10), sticky="nsew")

        self.table_a_frame = ttk.Frame(self.player_a_card)
        self.table_a_frame.pack(fill="both", expand=True)

        # Dice area for Player A
        self.dice_list_a_frame = ttk.Frame(self.root, style="Card.TFrame", padding=15)
        self.dice_list_a_frame.grid(row=1, column=1, padx=10, pady=(40, 10), sticky="nsew")

        # Possible moves for Player A
        self.possible_moves_a_frame = ttk.Frame(self.root, style="Card.TFrame", padding=15)
        self.possible_moves_a_frame.grid(row=1, column=2, padx=20, pady=(40, 10), sticky="nsew")

        # Separator
        separator = ttk.Separator(self.root, orient="horizontal")
        separator.grid(row=2, column=0, columnspan=3, sticky="ew", padx=30, pady=10)

        # Player B section
        self.player_b_label = ttk.Label(
            self.root,
            text="  Player B",
            style="Inactive.TLabel"
        )
        self.player_b_label.grid(row=3, column=0, padx=20, pady=10, sticky="n")

        # Player B card frame
        self.player_b_card = ttk.Frame(self.root, style="Card.TFrame", padding=15)
        self.player_b_card.grid(row=3, column=0, padx=20, pady=(40, 20), sticky="nsew")

        self.table_b_frame = ttk.Frame(self.player_b_card)
        self.table_b_frame.pack(fill="both", expand=True)

        # Dice area for Player B
        self.dice_list_b_frame = ttk.Frame(self.root, style="Card.TFrame", padding=15)
        self.dice_list_b_frame.grid(row=3, column=1, padx=10, pady=(40, 20), sticky="nsew")

        # Possible moves for Player B
        self.possible_moves_b_frame = ttk.Frame(self.root, style="Card.TFrame", padding=15)
        self.possible_moves_b_frame.grid(row=3, column=2, padx=20, pady=(40, 20), sticky="nsew")

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
        # Update player labels to show active player
        if self.game.current_player == self.game.player_a:
            self.player_a_label.config(text="▶ Player A (Your Turn)", style="Active.TLabel")
            self.player_b_label.config(text="  Player B", style="Inactive.TLabel")
        else:
            self.player_a_label.config(text="  Player A", style="Inactive.TLabel")
            self.player_b_label.config(text="▶ Player B (Your Turn)", style="Active.TLabel")

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
