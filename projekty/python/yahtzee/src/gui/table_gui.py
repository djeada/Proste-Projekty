import tkinter as tk

from src.game_logic.table import Table, ScoreType


class TableGui:
    def __init__(self, frame: tk.Frame, table: Table):
        self.frame = frame
        self.table = table
        self.draw()

    def draw(self) -> None:
        # add grid with two columns each with label and score
        for i, score_type in enumerate(ScoreType):
            label = tk.Label(self.frame, text=score_type.name)
            label.grid(row=i, column=0)
            score = tk.Label(self.frame, text=self.table.get_score(score_type))
            score.grid(row=i, column=1, padx=10)