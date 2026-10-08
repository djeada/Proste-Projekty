"""Tkinter window for Yahtzee. Click a die to hold it, select a row and press Score."""

import random
import tkinter as tk
import tkinter.font
from tkinter import simpledialog, ttk

from yahtzee import (
    CATEGORY_NAMES,
    MAX_ROLLS,
    NUM_DICE,
    NUM_ROUNDS,
    UPPER_BONUS_LIMIT,
    Category,
    Game,
    card_total,
    score_for,
    upper_bonus,
)

BONUS_ROW = "bonus"
TOTAL_ROW = "total"


def roll_face():
    return random.randint(1, 6)


class YahtzeeApp:
    def __init__(self, root, num_players):
        self.root = root
        self.game = Game(num_players)
        root.title("Yahtzee")

        big = tkinter.font.nametofont("TkDefaultFont").copy()
        big.configure(size=16, weight="bold")

        self.status = tk.Label(root, font=big)
        self.status.pack(pady=(10, 4))

        dice_frame = tk.Frame(root)
        dice_frame.pack(pady=6)
        self.dice_buttons = []
        for die in range(NUM_DICE):
            button = tk.Button(dice_frame, width=4, height=2, font=big, command=lambda d=die: self.toggle(d))
            button.pack(side=tk.LEFT, padx=4)
            self.dice_buttons.append(button)
        self.default_bg = self.dice_buttons[0].cget("bg")

        tk.Button(root, text="Roll", font=big, command=self.roll).pack(pady=4)

        self.tree = self.build_score_card(root, num_players)
        self.tree.pack(padx=10, pady=6)

        tk.Button(root, text="Score selected row", command=self.score_selected).pack(pady=4)
        self.message = tk.Label(root, wraplength=420, text="Click a die to hold it, roll again, then score a row.")
        self.message.pack(pady=(4, 10))

        self.refresh()

    def build_score_card(self, root, num_players):
        columns = ["category"] + [f"player{p}" for p in range(num_players)]
        tree = ttk.Treeview(root, columns=columns, show="headings", height=len(CATEGORY_NAMES) + 2)
        tree.heading("category", text="Category")
        tree.column("category", width=170)
        for p in range(num_players):
            tree.heading(f"player{p}", text=f"Player {p + 1}")
            tree.column(f"player{p}", width=90, anchor=tk.CENTER)
        for category in Category:
            tree.insert("", tk.END, iid=str(int(category)), values=[CATEGORY_NAMES[category]])
        tree.insert("", tk.END, iid=BONUS_ROW, values=[f"Upper bonus ({UPPER_BONUS_LIMIT}+)"])
        tree.insert("", tk.END, iid=TOTAL_ROW, values=["Total"])
        return tree

    def cell_text(self, player, category):
        score = self.game.cards[player][category]
        if score is not None:
            return str(score)
        if player == self.game.current and self.game.rolls > 0:
            return f"({score_for(self.game.dice, category)})"
        return "-"

    def refresh(self):
        game = self.game
        if game.is_over:
            winner = game.winner()
            text = f"Game over! Player {winner + 1} wins with {card_total(game.cards[winner])} points."
        else:
            text = f"Round {game.round} of {NUM_ROUNDS} - Player {game.current + 1} to play"
        self.status.config(text=text)

        for die, button in enumerate(self.dice_buttons):
            shown = "-" if game.rolls == 0 else str(game.dice[die])
            button.config(text=shown, bg="gold" if game.held[die] else self.default_bg)

        for category in Category:
            values = [CATEGORY_NAMES[category]]
            values += [self.cell_text(p, category) for p in range(len(game.cards))]
            self.tree.item(str(int(category)), values=values)
        self.tree.item(BONUS_ROW, values=[f"Upper bonus ({UPPER_BONUS_LIMIT}+)"] + [upper_bonus(c) for c in game.cards])
        self.tree.item(TOTAL_ROW, values=["Total"] + [card_total(c) for c in game.cards])

    def say(self, text):
        self.message.config(text=text)

    def roll(self):
        if self.game.roll(roll_face):
            self.say(f"Roll {self.game.rolls} of {MAX_ROLLS}. Hold the dice you like and roll again.")
        else:
            self.say("No rolls left. Select a row and press Score.")
        self.refresh()

    def toggle(self, die):
        if self.game.toggle_hold(die):
            self.say("")
        else:
            self.say("Roll the dice first.")
        self.refresh()

    def score_selected(self):
        selected = self.tree.selection()
        if not selected or selected[0] in (BONUS_ROW, TOTAL_ROW):
            self.say("Select a category row first.")
            return
        category = Category(int(selected[0]))
        if self.game.rolls == 0:
            self.say("Roll the dice first.")
        elif not self.game.choose(category):
            self.say("That category is already used. Pick another one.")
        else:
            self.say("")
        self.refresh()


def main():
    root = tk.Tk()
    num_players = simpledialog.askinteger(
        "Yahtzee", "How many players (1-4)?", parent=root, initialvalue=1, minvalue=1, maxvalue=4
    )
    if num_players is None:
        root.destroy()
        return
    YahtzeeApp(root, num_players)
    root.mainloop()


if __name__ == "__main__":
    main()
