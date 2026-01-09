"""
GUI for Snake game using tkinter.
"""
import tkinter as tk
from tkinter import ttk

import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from logic.game import SnakeGame, Direction


class Gui:
    """
    Main window for the Snake game.
    """

    CELL_SIZE = 20
    GAME_SPEED = 150  # milliseconds between moves

    def __init__(self, game: SnakeGame) -> None:
        self.root = tk.Tk()
        self.root.title("Snake")
        self.root.resizable(False, False)
        self.game = game
        self.canvas_width = game.width * self.CELL_SIZE
        self.canvas_height = game.height * self.CELL_SIZE
        self.setup()
        self.bind_keys()

    def run(self) -> None:
        """Start the main loop of the GUI."""
        self.game_loop()
        self.root.mainloop()

    def setup(self) -> None:
        """Create the frames and internal widgets."""
        # Score label
        self.score_label = ttk.Label(
            self.root,
            text=f"Score: {self.game.score}",
            font=("Arial", 14),
        )
        self.score_label.pack(pady=5)

        # Game canvas
        self.canvas = tk.Canvas(
            self.root,
            width=self.canvas_width,
            height=self.canvas_height,
            bg="black",
        )
        self.canvas.pack(padx=10, pady=10)

        # Restart button
        restart_button = ttk.Button(
            self.root, text="Restart", command=self.restart
        )
        restart_button.pack(pady=5)

    def bind_keys(self) -> None:
        """Bind keyboard controls."""
        self.root.bind("<Right>", lambda e: self.game.update_direction(Direction.RIGHT))
        self.root.bind("<Left>", lambda e: self.game.update_direction(Direction.LEFT))
        self.root.bind("<Up>", lambda e: self.game.update_direction(Direction.UP))
        self.root.bind("<Down>", lambda e: self.game.update_direction(Direction.DOWN))

    def game_loop(self) -> None:
        """Main game loop."""
        if not self.game.game_over:
            self.game.move()
            self.draw()
            self.update_score()
            self.root.after(self.GAME_SPEED, self.game_loop)
        else:
            self.show_game_over()

    def draw(self) -> None:
        """Draw the game state on the canvas."""
        self.canvas.delete("all")

        # Draw food
        food_x, food_y = self.game.food
        self.canvas.create_rectangle(
            food_x * self.CELL_SIZE,
            food_y * self.CELL_SIZE,
            (food_x + 1) * self.CELL_SIZE,
            (food_y + 1) * self.CELL_SIZE,
            fill="red",
        )

        # Draw snake head
        head_x, head_y = self.game.get_snake_head()
        self.canvas.create_rectangle(
            head_x * self.CELL_SIZE,
            head_y * self.CELL_SIZE,
            (head_x + 1) * self.CELL_SIZE,
            (head_y + 1) * self.CELL_SIZE,
            fill="darkgreen",
        )

        # Draw snake body
        for x, y in self.game.get_snake_body():
            self.canvas.create_rectangle(
                x * self.CELL_SIZE,
                y * self.CELL_SIZE,
                (x + 1) * self.CELL_SIZE,
                (y + 1) * self.CELL_SIZE,
                fill="green",
            )

    def update_score(self) -> None:
        """Update score label."""
        self.score_label.config(text=f"Score: {self.game.score}")

    def show_game_over(self) -> None:
        """Display game over message."""
        self.canvas.create_text(
            self.canvas_width // 2,
            self.canvas_height // 2,
            text="GAME OVER",
            fill="white",
            font=("Arial", 24, "bold"),
        )

    def restart(self) -> None:
        """Restart the game."""
        self.game.reset()
        self.game_loop()
