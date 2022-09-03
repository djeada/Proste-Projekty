import tkinter as tk
from typing import List

from src.game_logic.dice import Dice


class DiceGui:
    def __init__(self, frame: tk.Frame, dices: List[Dice], dice_put_away: List[int], parent_gui):
        self.frame = frame
        self.dices = dices
        self.dice_put_away = dice_put_away
        self.parent_gui = parent_gui
        self.upper_row_dices = dices.copy()
        self.lower_row_dices = [dice for i, dice in enumerate(dices) if i in dice_put_away]
        self.buttons = []
        self.draw()

    def draw(self) -> None:
        self.lower_row_dices = [dice for i, dice in enumerate(self.dices) if i in  self.dice_put_away]
        self.upper_row_dices = [dice for dice in self.dices if dice not in  self.lower_row_dices]
        # remove every child of self.frame
        for child in self.frame.winfo_children():
            child.destroy()
        self.buttons.clear()
        # draw each dice as a button with text of value
        label = tk.Label(self.frame, text="Rolled:")
        label.grid(row=0, column=0, pady=10)
        for i, dice in enumerate(self.upper_row_dices):
            button = tk.Button(self.frame, text=dice.value)
            button.grid(row=1, column=1 + i, padx=10)
            button.config(command=lambda dice=dice: self.move_dice_down(dice))
            self.buttons.append(button)

        label = tk.Label(self.frame, text="Put down:")
        label.grid(row=2, column=0, pady=10)
        for i, dice in enumerate(self.lower_row_dices):
            button = tk.Button(self.frame, text=dice.value)
            button.grid(row=3, column=1 + i, padx=10)
            button.config(command=lambda dice=dice: self.move_dice_up(dice))
            self.buttons.append(button)

    def move_dice_down(self, dice: Dice) -> None:
        self.lower_row_dices.append(dice)
        self.upper_row_dices.remove(dice)
        self.dice_put_away.clear()
        self.dice_put_away.extend([i for i, dice in enumerate(self.dices) if dice in self.lower_row_dices])
        self.parent_gui.draw()

    def move_dice_up(self, dice: Dice) -> None:
        self.upper_row_dices.append(dice)
        self.lower_row_dices.remove(dice)
        self.dice_put_away.clear()
        self.dice_put_away.extend([i for i, dice in enumerate(self.dices) if dice in self.lower_row_dices])
        self.parent_gui.draw()
