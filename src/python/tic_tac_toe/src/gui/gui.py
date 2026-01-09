"""
GUI for Tic-Tac-Toe game using tkinter.
"""
import tkinter as tk
from tkinter import ttk, messagebox
from typing import List

import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from logic.board import Board, PLAYER_X, PLAYER_O, BOARD_SIZE


class Gui:
    """
    Main window for the Tic-Tac-Toe game.
    """

    def __init__(self, board: Board, vs_ai: bool = True) -> None:
        self.root = tk.Tk()
        self.root.title("Tic-Tac-Toe")
        self.root.resizable(False, False)
        self.board = board
        self.vs_ai = vs_ai
        self.buttons: List[List[tk.Button]] = []
        self.setup()

    def run(self) -> None:
        """Start the main loop of the GUI."""
        self.root.mainloop()

    def setup(self) -> None:
        """Create the frames and internal widgets."""
        # Status label
        self.status_label = ttk.Label(
            self.root,
            text=f"Player {self.board.current_player}'s turn",
            font=("Arial", 14),
        )
        self.status_label.pack(pady=10)

        # Game board frame
        board_frame = ttk.Frame(self.root)
        board_frame.pack(padx=10, pady=10)

        self.buttons = []
        for i in range(BOARD_SIZE):
            row_buttons = []
            for j in range(BOARD_SIZE):
                button = tk.Button(
                    board_frame,
                    text="",
                    font=("Arial", 24, "bold"),
                    width=4,
                    height=2,
                    command=lambda r=i, c=j: self.on_click(r, c),
                )
                button.grid(row=i, column=j, padx=2, pady=2)
                row_buttons.append(button)
            self.buttons.append(row_buttons)

        # Restart button
        restart_button = ttk.Button(
            self.root, text="Restart", command=self.restart
        )
        restart_button.pack(pady=10)

    def on_click(self, row: int, col: int) -> None:
        """Handle button click."""
        if self.board.game_over:
            return

        if not self.board.make_move(row, col):
            return

        self.update_button(row, col)

        winner = self.board.check_winner()
        if winner:
            self.handle_win(winner)
            return

        if self.board.is_board_full():
            self.handle_draw()
            return

        self.board.switch_player()
        self.update_status()

        # AI move
        if self.vs_ai and self.board.current_player == PLAYER_O:
            self.root.after(500, self.ai_move)

    def ai_move(self) -> None:
        """Make AI move."""
        if self.board.game_over:
            return

        move = self.board.ai_make_move()
        if move:
            row, col = move
            self.board.make_move(row, col)
            self.update_button(row, col)

            winner = self.board.check_winner()
            if winner:
                self.handle_win(winner)
                return

            if self.board.is_board_full():
                self.handle_draw()
                return

            self.board.switch_player()
            self.update_status()

    def update_button(self, row: int, col: int) -> None:
        """Update button display."""
        player = self.board.cells[row][col]
        color = "blue" if player == PLAYER_X else "red"
        self.buttons[row][col].config(text=player, fg=color, state="disabled")

    def update_status(self) -> None:
        """Update status label."""
        self.status_label.config(text=f"Player {self.board.current_player}'s turn")

    def handle_win(self, winner: str) -> None:
        """Handle game win."""
        self.board.game_over = True
        self.board.winner = winner

        # Highlight winning line
        winning_line = self.board.get_winning_line()
        if winning_line:
            for row, col in winning_line:
                self.buttons[row][col].config(bg="lightgreen")

        self.status_label.config(text=f"Player {winner} wins!")
        self.disable_all_buttons()

    def handle_draw(self) -> None:
        """Handle game draw."""
        self.board.game_over = True
        self.status_label.config(text="It's a draw!")
        self.disable_all_buttons()

    def disable_all_buttons(self) -> None:
        """Disable all board buttons."""
        for row in self.buttons:
            for button in row:
                button.config(state="disabled")

    def restart(self) -> None:
        """Restart the game."""
        self.board.reset()
        for i in range(BOARD_SIZE):
            for j in range(BOARD_SIZE):
                self.buttons[i][j].config(
                    text="", state="normal", bg="SystemButtonFace", fg="black"
                )
        self.update_status()
