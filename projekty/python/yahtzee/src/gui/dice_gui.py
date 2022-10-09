import tkinter as tk
from tkinter import ttk
from typing import List

from projekty.python.yahtzee.src.logic.dice import Dice


class DiceGui:
    """
    A frame with buttons representing dice.
    """

    def __init__(
        self,
        frame: ttk.Frame,
        dice_list: List[Dice],
        dice_put_away: List[int],
        parent_gui,
    ):
        self.frame = frame
        self.dice_list = dice_list
        self.dice_put_away = dice_put_away
        self.parent_gui = parent_gui
        self.upper_row_dice_list = dice_list.copy()
        self.lower_row_dice_list = [
            dice for i, dice in enumerate(dice_list) if i in dice_put_away
        ]
        self.buttons = []
        self.draw()

    def draw(self) -> None:
        """
        Draws the frame.
        """
        self.lower_row_dice_list = [
            dice for i, dice in enumerate(self.dice_list) if i in self.dice_put_away
        ]
        self.upper_row_dice_list = [
            dice for dice in self.dice_list if dice not in self.lower_row_dice_list
        ]
        # remove every child of self.frame
        for child in self.frame.winfo_children():
            child.destroy()
        self.buttons.clear()
        # draw each dice as a button with text of value
        label = ttk.Label(self.frame, text="Rolled:")
        label.grid(row=0, column=0, pady=10)
        for i, dice in enumerate(self.upper_row_dice_list):
            button = ttk.Button(self.frame, text=dice.value)
            button.grid(row=1, column=1 + i, padx=10)
            button.config(command=lambda dice=dice: self.move_dice_down(dice))
            self.buttons.append(button)

        label = ttk.Label(self.frame, text="Put down:")
        label.grid(row=2, column=0, pady=10)
        for i, dice in enumerate(self.lower_row_dice_list):
            button = ttk.Button(self.frame, text=dice.value)
            button.grid(row=3, column=1 + i, padx=10)
            button.config(command=lambda dice=dice: self.move_dice_up(dice))
            self.buttons.append(button)

    def move_dice_down(self, dice: Dice) -> None:
        """
        Moves dice from upper row to lower row.

        :param dice: Dice to move.
        """
        self.lower_row_dice_list.append(dice)
        self.upper_row_dice_list.remove(dice)
        self.dice_put_away.clear()
        self.dice_put_away.extend(
            [
                i
                for i, dice in enumerate(self.dice_list)
                if dice in self.lower_row_dice_list
            ]
        )
        self.parent_gui.draw()

    def move_dice_up(self, dice: Dice) -> None:
        """
        Moves dice from lower row to upper row.

        :param dice: Dice to move.
        """
        self.upper_row_dice_list.append(dice)
        self.lower_row_dice_list.remove(dice)
        self.dice_put_away.clear()
        self.dice_put_away.extend(
            [
                i
                for i, dice in enumerate(self.dice_list)
                if dice in self.lower_row_dice_list
            ]
        )
        self.parent_gui.draw()
