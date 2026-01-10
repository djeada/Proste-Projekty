import tkinter as tk
from tkinter import ttk
from typing import List

try:
    from logic.dice import Dice
except ImportError:
    from src.logic.dice import Dice


# Dice face representations using Unicode
DICE_FACES = {
    -1: "?",
    1: "⚀",
    2: "⚁",
    3: "⚂",
    4: "⚃",
    5: "⚄",
    6: "⚅",
}


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

        # Clear existing widgets
        for child in self.frame.winfo_children():
            child.destroy()
        self.buttons.clear()

        # Header
        header = ttk.Label(
            self.frame,
            text="🎲 Dice",
            style="SubHeader.TLabel"
        )
        header.grid(row=0, column=0, columnspan=6, pady=(0, 15), sticky="w")

        # Active dice section
        active_label = ttk.Label(
            self.frame,
            text="🔄 Roll again:",
            style="DiceLabel.TLabel"
        )
        active_label.grid(row=1, column=0, pady=(0, 5), sticky="w")

        # Active dice buttons (dice to be rolled)
        active_frame = ttk.Frame(self.frame)
        active_frame.grid(row=2, column=0, columnspan=6, pady=(0, 15), sticky="w")

        for i, dice in enumerate(self.upper_row_dice_list):
            dice_face = DICE_FACES.get(dice.value, "?")
            button = tk.Button(
                active_frame,
                text=dice_face,
                font=("Helvetica", 28),
                width=2,
                height=1,
                bg="#ffffff",
                fg="#2196F3",
                activebackground="#e3f2fd",
                activeforeground="#1976D2",
                relief="solid",
                borderwidth=2,
                cursor="hand2",
                command=lambda d=dice: self.move_dice_down(d)
            )
            button.pack(side="left", padx=5)
            self.buttons.append(button)

        # Separator
        if self.upper_row_dice_list or self.lower_row_dice_list:
            ttk.Separator(self.frame, orient="horizontal").grid(
                row=3, column=0, columnspan=6, sticky="ew", pady=10
            )

        # Kept dice section
        kept_label = ttk.Label(
            self.frame,
            text="✓ Keep these:",
            style="DiceLabel.TLabel"
        )
        kept_label.grid(row=4, column=0, pady=(0, 5), sticky="w")

        kept_frame = ttk.Frame(self.frame)
        kept_frame.grid(row=5, column=0, columnspan=6, pady=(0, 10), sticky="w")

        for i, dice in enumerate(self.lower_row_dice_list):
            dice_face = DICE_FACES.get(dice.value, "?")
            button = tk.Button(
                kept_frame,
                text=dice_face,
                font=("Helvetica", 28),
                width=2,
                height=1,
                bg="#e8f5e9",
                fg="#4CAF50",
                activebackground="#c8e6c9",
                activeforeground="#388E3C",
                relief="solid",
                borderwidth=2,
                cursor="hand2",
                command=lambda d=dice: self.move_dice_up(d)
            )
            button.pack(side="left", padx=5)
            self.buttons.append(button)

        # Instructions
        if self.upper_row_dice_list or self.lower_row_dice_list:
            hint = ttk.Label(
                self.frame,
                text="💡 Click dice to move between sections",
                font=("Helvetica", 9, "italic"),
                foreground="#9e9e9e"
            )
            hint.grid(row=6, column=0, columnspan=6, pady=(10, 0), sticky="w")

    def move_dice_down(self, dice: Dice) -> None:
        """
        Moves dice from upper row to lower row (keep this dice).

        :param dice: Dice to move.
        """
        self.lower_row_dice_list.append(dice)
        self.upper_row_dice_list.remove(dice)
        self.dice_put_away.clear()
        self.dice_put_away.extend(
            [
                i
                for i, d in enumerate(self.dice_list)
                if d in self.lower_row_dice_list
            ]
        )
        self.parent_gui.draw()

    def move_dice_up(self, dice: Dice) -> None:
        """
        Moves dice from lower row to upper row (roll this dice again).

        :param dice: Dice to move.
        """
        self.upper_row_dice_list.append(dice)
        self.lower_row_dice_list.remove(dice)
        self.dice_put_away.clear()
        self.dice_put_away.extend(
            [
                i
                for i, d in enumerate(self.dice_list)
                if d in self.lower_row_dice_list
            ]
        )
        self.parent_gui.draw()
