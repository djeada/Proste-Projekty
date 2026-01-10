"""
GUI for 2048 game using tkinter.
"""
import tkinter as tk
from tkinter import ttk

import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from gui.styled_window import StyledWindow
from logic.game import Game2048, Move, GRID_SIZE


# Color scheme for tiles
TILE_COLORS = {
    0: "#cdc1b4",
    2: "#eee4da",
    4: "#ede0c8",
    8: "#f2b179",
    16: "#f59563",
    32: "#f67c5f",
    64: "#f65e3b",
    128: "#edcf72",
    256: "#edcc61",
    512: "#edc850",
    1024: "#edc53f",
    2048: "#edc22e",
}

TEXT_COLORS = {
    0: "#cdc1b4",
    2: "#776e65",
    4: "#776e65",
    8: "#f9f6f2",
    16: "#f9f6f2",
    32: "#f9f6f2",
    64: "#f9f6f2",
    128: "#f9f6f2",
    256: "#f9f6f2",
    512: "#f9f6f2",
    1024: "#f9f6f2",
    2048: "#f9f6f2",
}


class Gui:
    """
    Main window for the 2048 game.
    """

    CELL_SIZE = 100
    CELL_PAD = 10

    def __init__(self, game: Game2048) -> None:
        self.root = StyledWindow()
        self.root.title("2048")
        self.root.resizable(False, False)
        self.game = game
        self.labels = []
        self.setup()
        self.bind_keys()
        self.update_display()

    def run(self) -> None:
        """Start the main loop of the GUI."""
        self.root.mainloop()

    def setup(self) -> None:
        """Create the frames and internal widgets."""
        # Score label
        self.score_label = ttk.Label(
            self.root,
            text=f"Score: {self.game.score}",
            font=("Arial", 16, "bold"),
        )
        self.score_label.pack(pady=10)

        # Game board frame
        board_frame = tk.Frame(self.root, bg="#bbada0")
        board_frame.pack(padx=10, pady=10)

        # Create tile labels
        self.labels = []
        for row in range(GRID_SIZE):
            row_labels = []
            for col in range(GRID_SIZE):
                cell_frame = tk.Frame(
                    board_frame,
                    width=self.CELL_SIZE,
                    height=self.CELL_SIZE,
                )
                cell_frame.grid(
                    row=row, column=col, padx=self.CELL_PAD // 2, pady=self.CELL_PAD // 2
                )
                cell_frame.pack_propagate(False)

                label = tk.Label(
                    cell_frame,
                    text="",
                    font=("Arial", 24, "bold"),
                    justify="center",
                )
                label.pack(expand=True, fill="both")
                row_labels.append(label)
            self.labels.append(row_labels)

        # Restart button
        restart_button = ttk.Button(
            self.root, text="New Game", command=self.restart
        )
        restart_button.pack(pady=10)

        # Status label
        self.status_label = ttk.Label(
            self.root,
            text="Use arrow keys to move",
            font=("Arial", 12),
        )
        self.status_label.pack(pady=5)

    def bind_keys(self) -> None:
        """Bind keyboard controls."""
        self.root.bind("<Up>", lambda e: self.handle_move(Move.UP))
        self.root.bind("<Down>", lambda e: self.handle_move(Move.DOWN))
        self.root.bind("<Left>", lambda e: self.handle_move(Move.LEFT))
        self.root.bind("<Right>", lambda e: self.handle_move(Move.RIGHT))
        # WASD controls
        self.root.bind("<w>", lambda e: self.handle_move(Move.UP))
        self.root.bind("<s>", lambda e: self.handle_move(Move.DOWN))
        self.root.bind("<a>", lambda e: self.handle_move(Move.LEFT))
        self.root.bind("<d>", lambda e: self.handle_move(Move.RIGHT))

    def handle_move(self, direction: Move) -> None:
        """Handle a move in the specified direction."""
        if self.game.game_over:
            return

        self.game.move(direction)
        self.update_display()

        if self.game.won:
            self.status_label.config(text="You Win! Keep playing or restart.")
        elif self.game.game_over:
            self.status_label.config(text="Game Over! Press New Game to restart.")

    def update_display(self) -> None:
        """Update the display with current game state."""
        self.score_label.config(text=f"Score: {self.game.score}")

        for row in range(GRID_SIZE):
            for col in range(GRID_SIZE):
                value = self.game.cells[row][col]
                label = self.labels[row][col]

                if value == 0:
                    label.config(text="", bg=TILE_COLORS[0])
                else:
                    color_val = min(value, 2048)
                    bg_color = TILE_COLORS.get(color_val, "#3c3a32")
                    fg_color = TEXT_COLORS.get(color_val, "#f9f6f2")
                    label.config(text=str(value), bg=bg_color, fg=fg_color)

    def restart(self) -> None:
        """Restart the game."""
        self.game.reset()
        self.status_label.config(text="Use arrow keys to move")
        self.update_display()
