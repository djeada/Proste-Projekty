import tkinter as tk
from tkinter import ttk

try:
    from logic.table import ScoreType, Table
except ImportError:
    from src.logic.table import ScoreType, Table


# Map score types to user-friendly names
SCORE_DISPLAY_NAMES = {
    ScoreType.ACES: "Aces (1s)",
    ScoreType.TWOS: "Twos (2s)",
    ScoreType.THREES: "Threes (3s)",
    ScoreType.FOURS: "Fours (4s)",
    ScoreType.FIVES: "Fives (5s)",
    ScoreType.SIXES: "Sixes (6s)",
    ScoreType.THREE_OF_A_KIND: "Three of a Kind",
    ScoreType.FOUR_OF_A_KIND: "Four of a Kind",
    ScoreType.FULL_HOUSE: "Full House",
    ScoreType.SMALL_STRAIGHT: "Small Straight",
    ScoreType.LARGE_STRAIGHT: "Large Straight",
    ScoreType.CHANCE: "Chance",
    ScoreType.YAHTZEE: "YAHTZEE!",
}


class TableGui:
    """
    GUI for the score table.
    """

    def __init__(self, frame: ttk.Frame, table: Table):
        self.frame = frame
        self.table = table
        self.labels = {}
        self.score_labels = {}
        self._setup()

    def _setup(self) -> None:
        """
        Sets up the static table structure.
        """
        # Header
        header = ttk.Label(
            self.frame,
            text="📊 Scorecard",
            style="SubHeader.TLabel"
        )
        header.grid(row=0, column=0, columnspan=2, pady=(0, 10), sticky="w")

        # Upper section header
        upper_header = ttk.Label(
            self.frame,
            text="Upper Section",
            font=("Helvetica", 9, "bold"),
            foreground="#666666"
        )
        upper_header.grid(row=1, column=0, columnspan=2, pady=(5, 3), sticky="w")

        # Score rows
        row_idx = 2
        upper_section = [
            ScoreType.ACES, ScoreType.TWOS, ScoreType.THREES,
            ScoreType.FOURS, ScoreType.FIVES, ScoreType.SIXES
        ]
        lower_section = [
            ScoreType.THREE_OF_A_KIND, ScoreType.FOUR_OF_A_KIND,
            ScoreType.FULL_HOUSE, ScoreType.SMALL_STRAIGHT,
            ScoreType.LARGE_STRAIGHT, ScoreType.CHANCE, ScoreType.YAHTZEE
        ]

        for score_type in upper_section:
            self._add_score_row(score_type, row_idx)
            row_idx += 1

        # Lower section header
        lower_header = ttk.Label(
            self.frame,
            text="Lower Section",
            font=("Helvetica", 9, "bold"),
            foreground="#666666"
        )
        lower_header.grid(row=row_idx, column=0, columnspan=2, pady=(10, 3), sticky="w")
        row_idx += 1

        for score_type in lower_section:
            self._add_score_row(score_type, row_idx)
            row_idx += 1

        # Total row
        ttk.Separator(self.frame, orient="horizontal").grid(
            row=row_idx, column=0, columnspan=2, sticky="ew", pady=5
        )
        row_idx += 1

        total_label = ttk.Label(
            self.frame,
            text="TOTAL",
            font=("Helvetica", 11, "bold")
        )
        total_label.grid(row=row_idx, column=0, sticky="w", pady=2)

        self.total_label = ttk.Label(
            self.frame,
            text="0",
            style="ScoreValue.TLabel",
            font=("Helvetica", 11, "bold")
        )
        self.total_label.grid(row=row_idx, column=1, sticky="e", pady=2)

    def _add_score_row(self, score_type: ScoreType, row: int) -> None:
        """Add a single score row to the table."""
        display_name = SCORE_DISPLAY_NAMES.get(score_type, score_type.name)

        label = ttk.Label(
            self.frame,
            text=display_name,
            style="Score.TLabel"
        )
        label.grid(row=row, column=0, sticky="w", pady=1)
        self.labels[score_type] = label

        score_label = ttk.Label(
            self.frame,
            text="-",
            style="ScoreValue.TLabel"
        )
        score_label.grid(row=row, column=1, sticky="e", pady=1, padx=(20, 0))
        self.score_labels[score_type] = score_label

    def draw(self) -> None:
        """
        Updates the score values.
        """
        for score_type, label in self.score_labels.items():
            score = self.table.get_score(score_type)
            if score > 0:
                label.config(text=str(score))
            else:
                label.config(text="-")

        self.total_label.config(text=str(self.table.total()))
