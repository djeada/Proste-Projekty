import tkinter as tk
from tkinter import ttk

from projekty.python.yahtzee.src.logic.table import ScoreType, Table


class TableGui:
    """
    GUI for the score table.
    """

    def __init__(self, frame: ttk.Frame, table: Table):
        self.frame = frame
        self.table = table
        self.draw()

    def draw(self) -> None:
        """
        Draws the table.
        """
        # add grid with two columns each with label and score
        for i, score_type in enumerate(ScoreType):
            label = ttk.Label(self.frame, text=score_type.name)
            label.grid(row=i, column=0)
            score = ttk.Label(self.frame, text=self.table.get_score(score_type))
            score.grid(row=i, column=1, padx=10)
