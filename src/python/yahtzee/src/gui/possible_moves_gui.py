import tkinter as tk
from tkinter import ttk

try:
    from logic.game import Game
    from logic.player import Player
    from logic.table import ScoreType
except ImportError:
    from src.logic.game import Game
    from src.logic.player import Player
    from src.logic.table import ScoreType


# Map score types to user-friendly names
SCORE_DISPLAY_NAMES = {
    ScoreType.ACES: "Aces",
    ScoreType.TWOS: "Twos",
    ScoreType.THREES: "Threes",
    ScoreType.FOURS: "Fours",
    ScoreType.FIVES: "Fives",
    ScoreType.SIXES: "Sixes",
    ScoreType.THREE_OF_A_KIND: "3 of a Kind",
    ScoreType.FOUR_OF_A_KIND: "4 of a Kind",
    ScoreType.FULL_HOUSE: "Full House",
    ScoreType.SMALL_STRAIGHT: "Sm. Straight",
    ScoreType.LARGE_STRAIGHT: "Lg. Straight",
    ScoreType.CHANCE: "Chance",
    ScoreType.YAHTZEE: "YAHTZEE!",
}


class PossibleMovesGui:
    """
    GUI for the possible moves.
    """

    def __init__(self, frame: ttk.Frame, player: Player, game: Game, parent_gui):
        self.frame = frame
        self.player = player
        self.game = game
        self.parent_gui = parent_gui
        self.draw()

    def draw(self) -> None:
        """
        Draws all the widgets.
        """
        # Clear existing widgets
        for child in self.frame.winfo_children():
            child.destroy()

        # If it's not this player's turn, show waiting message
        if self.game.current_player != self.player:
            waiting_label = ttk.Label(
                self.frame,
                text="⏳ Waiting...",
                style="SubHeader.TLabel",
                foreground="#9e9e9e"
            )
            waiting_label.pack(pady=20)
            return

        # Header
        header = ttk.Label(
            self.frame,
            text="🎯 Actions",
            style="SubHeader.TLabel"
        )
        header.pack(pady=(0, 10), anchor="w")

        # Throws remaining indicator
        throws_left = self.player.numbers_of_throws_left
        throws_text = f"🎲 Rolls remaining: {throws_left}"
        throws_color = "#4CAF50" if throws_left > 1 else "#FF9800" if throws_left == 1 else "#f44336"

        throws_label = ttk.Label(
            self.frame,
            text=throws_text,
            font=("Helvetica", 10, "bold"),
            foreground=throws_color
        )
        throws_label.pack(pady=(0, 15), anchor="w")

        # Roll dice button (prominent)
        if throws_left > 0:
            roll_button = tk.Button(
                self.frame,
                text="🎲 Roll Dice",
                font=("Helvetica", 12, "bold"),
                bg="#2196F3",
                fg="white",
                activebackground="#1976D2",
                activeforeground="white",
                relief="flat",
                padx=20,
                pady=10,
                cursor="hand2",
                command=self.parent_gui.roll_dice
            )
            roll_button.pack(fill="x", pady=(0, 15))

        # Available scoring options
        valid_rules = self.player.valid_rules()
        if valid_rules:
            options_label = ttk.Label(
                self.frame,
                text="📝 Score options:",
                font=("Helvetica", 10),
                foreground="#666666"
            )
            options_label.pack(pady=(5, 5), anchor="w")

            for score_type in valid_rules:
                display_name = SCORE_DISPLAY_NAMES.get(score_type, score_type.name)

                # Special styling for Yahtzee
                if score_type == ScoreType.YAHTZEE:
                    bg_color = "#FFD700"
                    fg_color = "#000000"
                else:
                    bg_color = "#ffffff"
                    fg_color = "#333333"

                button = tk.Button(
                    self.frame,
                    text=display_name,
                    font=("Helvetica", 10),
                    bg=bg_color,
                    fg=fg_color,
                    activebackground="#e0e0e0",
                    relief="solid",
                    borderwidth=1,
                    padx=10,
                    pady=5,
                    cursor="hand2",
                    command=lambda st=score_type: self.update_table(st)
                )
                button.pack(fill="x", pady=2)

    def update_table(self, score_type: ScoreType) -> None:
        """
        Updates the table with the score_type.

        :param score_type: score_type to update the table with
        """
        self.game.update_current_player_table(score_type)
        self.parent_gui.draw()
