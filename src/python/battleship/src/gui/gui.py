"""
GUI for Battleship game using tkinter.
"""
import tkinter as tk
from tkinter import ttk, messagebox

import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from logic.game import BattleshipGame, CellState, BOARD_SIZE


class Gui:
    """
    Main window for the Battleship game.
    """

    CELL_SIZE = 30

    def __init__(self, game: BattleshipGame) -> None:
        self.root = tk.Tk()
        self.root.title("Battleship")
        self.root.resizable(False, False)
        self.game = game
        self.player_buttons = []
        self.enemy_buttons = []
        self.setup()

    def run(self) -> None:
        """Start the main loop of the GUI."""
        self.root.mainloop()

    def setup(self) -> None:
        """Create the frames and internal widgets."""
        # Title
        title_label = ttk.Label(
            self.root, text="Battleship", font=("Arial", 18, "bold")
        )
        title_label.pack(pady=10)

        # Main game area
        game_frame = ttk.Frame(self.root)
        game_frame.pack(padx=10, pady=10)

        # Player board (left)
        player_frame = ttk.LabelFrame(game_frame, text="Your Fleet")
        player_frame.grid(row=0, column=0, padx=10)

        self.player_buttons = []
        for y in range(BOARD_SIZE):
            row_buttons = []
            for x in range(BOARD_SIZE):
                btn = tk.Button(
                    player_frame,
                    width=2,
                    height=1,
                    bg="lightblue",
                )
                btn.grid(row=y, column=x, padx=1, pady=1)
                row_buttons.append(btn)
            self.player_buttons.append(row_buttons)

        # Enemy board (right)
        enemy_frame = ttk.LabelFrame(game_frame, text="Enemy Waters")
        enemy_frame.grid(row=0, column=1, padx=10)

        self.enemy_buttons = []
        for y in range(BOARD_SIZE):
            row_buttons = []
            for x in range(BOARD_SIZE):
                btn = tk.Button(
                    enemy_frame,
                    width=2,
                    height=1,
                    bg="lightgray",
                    command=lambda x=x, y=y: self.on_enemy_cell_click(x, y),
                )
                btn.grid(row=y, column=x, padx=1, pady=1)
                row_buttons.append(btn)
            self.enemy_buttons.append(row_buttons)

        # Status and controls
        control_frame = ttk.Frame(self.root)
        control_frame.pack(pady=10)

        self.status_label = ttk.Label(
            control_frame, text="Your turn - Click on enemy waters to fire!"
        )
        self.status_label.pack()

        restart_button = ttk.Button(
            control_frame, text="New Game", command=self.restart
        )
        restart_button.pack(pady=5)

        # Update display with initial ship positions
        self.update_player_board()

    def update_player_board(self) -> None:
        """Update the player's board display."""
        for y in range(BOARD_SIZE):
            for x in range(BOARD_SIZE):
                cell = self.game.player_board.grid[y][x]
                btn = self.player_buttons[y][x]

                if cell == CellState.SHIP:
                    btn.config(bg="darkgray")
                elif cell == CellState.HIT:
                    btn.config(bg="red")
                elif cell == CellState.MISS:
                    btn.config(bg="white")
                else:
                    btn.config(bg="lightblue")

    def update_enemy_board(self) -> None:
        """Update the enemy's board display (only showing hits/misses)."""
        for y in range(BOARD_SIZE):
            for x in range(BOARD_SIZE):
                cell = self.game.enemy_board.grid[y][x]
                btn = self.enemy_buttons[y][x]

                if cell == CellState.HIT:
                    btn.config(bg="red")
                elif cell == CellState.MISS:
                    btn.config(bg="white")
                else:
                    btn.config(bg="lightgray")

    def on_enemy_cell_click(self, x: int, y: int) -> None:
        """Handle click on enemy board."""
        if self.game.game_over:
            return

        if not self.game.player_turn:
            return

        hit, sunk_ship = self.game.player_fire(x, y)
        self.update_enemy_board()

        if self.game.game_over:
            self.status_label.config(text=f"{self.game.winner} wins!")
            messagebox.showinfo("Game Over", f"{self.game.winner} wins!")
            return

        if hit:
            if sunk_ship:
                self.status_label.config(text="You sunk a ship! Enemy's turn...")
            else:
                self.status_label.config(text="Hit! Enemy's turn...")
        else:
            self.status_label.config(text="Miss! Enemy's turn...")

        # Enemy's turn
        self.root.after(500, self.enemy_turn)

    def enemy_turn(self) -> None:
        """Process enemy's turn."""
        x, y, hit = self.game.enemy_fire()
        self.update_player_board()

        if self.game.game_over:
            self.status_label.config(text=f"{self.game.winner} wins!")
            messagebox.showinfo("Game Over", f"{self.game.winner} wins!")
            return

        if hit:
            self.status_label.config(
                text=f"Enemy hit at ({x}, {y})! Your turn..."
            )
        else:
            self.status_label.config(
                text=f"Enemy missed at ({x}, {y})! Your turn..."
            )

    def restart(self) -> None:
        """Restart the game."""
        self.game.reset()
        self.update_player_board()
        for y in range(BOARD_SIZE):
            for x in range(BOARD_SIZE):
                self.enemy_buttons[y][x].config(bg="lightgray")
        self.status_label.config(text="Your turn - Click on enemy waters to fire!")
