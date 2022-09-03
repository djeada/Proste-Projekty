import tkinter as tk

from src.game_logic.game import Game
from src.game_logic.player import Player
from src.game_logic.table import ScoreType


class PossibleMovesGui:
    def __init__(self, frame: tk.Frame, player: Player, game:Game, parent_gui):
        self.frame = frame
        self.player = player
        self.game = game
        self.parent_gui = parent_gui
        self.draw()

    def draw(self) -> None:
        # remove every child of self.frame
        for child in self.frame.winfo_children():
            child.destroy()

        # if it's not players turn don't draw anything
        if self.game.current_player != self.player:
            return

        # draw each possible move as a button
        i = -1
        for i, score_type in enumerate(self.player.valid_rules()):
            button = tk.Button(self.frame, text=score_type.name)
            button.grid(row=i, column=0, pady=10)
            button.config(command=lambda score_type=score_type: self.update_table(score_type))

        roll_button = tk.Button(self.frame, text="Roll dice")
        roll_button.config(command=self.parent_gui.roll_dices)
        roll_button.grid(row=i + 1, column=0, pady=10)

    def update_table(self, score_type: ScoreType) -> None:
        self.game.update_current_player_table(score_type)
        self.parent_gui.draw()
