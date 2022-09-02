import tkinter as tk

from src.game_logic.player import Player
from src.game_logic.table import ScoreType


class PossibleMovesGui:
    def __init__(self, frame: tk.Frame, player: Player):
        self.frame = frame
        self.player = player
        self.roll_button = tk.Button(self.frame, text="Roll dices again")
        self.draw()

    def draw(self) -> None:
        # remove every child of self.frame
        for child in self.frame.winfo_children():
            if child is self.roll_button:
                continue
            child.destroy()
        # draw each possible move as a button
        i = -1
        for i, score_type in enumerate(self.player.valid_rules()):
            button = tk.Button(self.frame, text=score_type.name)
            button.grid(row=i, column=0, pady=10)
            button.config(command=lambda score_type=score_type: self.update_table(score_type))

        self.roll_button.grid(row=i + 1, column=0, pady=10)

    def update_table(self, score_type: ScoreType) -> None:
        self.player.update_table(score_type)
