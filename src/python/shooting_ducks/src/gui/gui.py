"""
GUI for Shooting Ducks game using tkinter.
"""
import tkinter as tk
from tkinter import ttk

import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)
from logic.game import DuckGame


class Gui:
    """
    Main window for the Shooting Ducks game.
    """

    GAME_SPEED = 50  # milliseconds between updates

    def __init__(self, game: DuckGame) -> None:
        self.root = tk.Tk()
        self.root.title("Shooting Ducks")
        self.root.resizable(False, False)
        self.game = game
        self.setup()

    def run(self) -> None:
        """Start the main loop of the GUI."""
        self.game_loop()
        self.root.mainloop()

    def setup(self) -> None:
        """Create the frames and internal widgets."""
        # Status bar
        self.status_label = ttk.Label(
            self.root,
            text=self.game.get_status(),
            font=("Arial", 12),
        )
        self.status_label.pack(pady=5)

        # Game canvas
        self.canvas = tk.Canvas(
            self.root,
            width=self.game.width,
            height=self.game.height,
            bg="skyblue",
        )
        self.canvas.pack(padx=10, pady=10)

        # Bind click event
        self.canvas.bind("<Button-1>", self.on_click)

        # Control buttons
        control_frame = ttk.Frame(self.root)
        control_frame.pack(pady=5)

        pause_button = ttk.Button(
            control_frame, text="Pause (P)", command=self.game.toggle_pause
        )
        pause_button.grid(row=0, column=0, padx=5)

        restart_button = ttk.Button(
            control_frame, text="Restart", command=self.restart
        )
        restart_button.grid(row=0, column=1, padx=5)

        # Key bindings
        self.root.bind("<p>", lambda e: self.game.toggle_pause())
        self.root.bind("<P>", lambda e: self.game.toggle_pause())
        self.root.bind("<r>", lambda e: self.restart())
        self.root.bind("<R>", lambda e: self.restart())

    def game_loop(self) -> None:
        """Main game loop."""
        self.game.update()
        self.draw()
        self.update_status()
        self.root.after(self.GAME_SPEED, self.game_loop)

    def draw(self) -> None:
        """Draw the game state."""
        self.canvas.delete("all")

        # Draw ground
        self.canvas.create_rectangle(
            0, self.game.height - 50,
            self.game.width, self.game.height,
            fill="green",
        )

        # Draw ducks
        for duck in self.game.ducks:
            if duck.alive:
                # Draw duck body
                self.canvas.create_oval(
                    duck.x, duck.y,
                    duck.x + duck.width, duck.y + duck.height,
                    fill="orange",
                    outline="darkorange",
                )
                # Draw duck head
                self.canvas.create_oval(
                    duck.x + duck.width - 15, duck.y - 5,
                    duck.x + duck.width + 5, duck.y + 15,
                    fill="orange",
                    outline="darkorange",
                )
                # Draw beak
                self.canvas.create_polygon(
                    duck.x + duck.width + 5, duck.y + 5,
                    duck.x + duck.width + 15, duck.y + 5,
                    duck.x + duck.width + 5, duck.y + 10,
                    fill="yellow",
                )

        # Draw crosshair hint
        if not self.game.game_over:
            self.canvas.create_text(
                self.game.width // 2, 20,
                text="Click on ducks to shoot!",
                fill="white",
                font=("Arial", 10),
            )

        # Draw game over overlay
        if self.game.game_over:
            self.canvas.create_rectangle(
                0, 0, self.game.width, self.game.height,
                fill="black",
                stipple="gray50",
            )
            self.canvas.create_text(
                self.game.width // 2, self.game.height // 2 - 20,
                text="GAME OVER",
                fill="white",
                font=("Arial", 24, "bold"),
            )
            self.canvas.create_text(
                self.game.width // 2, self.game.height // 2 + 20,
                text=f"Final Score: {self.game.score}",
                fill="white",
                font=("Arial", 16),
            )
            self.canvas.create_text(
                self.game.width // 2, self.game.height // 2 + 50,
                text="Press R to restart",
                fill="white",
                font=("Arial", 12),
            )

    def update_status(self) -> None:
        """Update status label."""
        self.status_label.config(text=self.game.get_status())

    def on_click(self, event: tk.Event) -> None:
        """Handle mouse click."""
        self.game.shoot(event.x, event.y)

    def restart(self) -> None:
        """Restart the game."""
        self.game.reset()
