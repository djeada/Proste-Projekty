"""Tkinter window: pick a level, left-click to reveal a cell, right-click to flag it."""

import random
import tkinter as tk
import tkinter.font as tkfont

from minesweeper import Game

LEVELS = {
    "Beginner": (9, 9, 10),
    "Intermediate": (16, 16, 40),
    "Expert": (16, 30, 99),
}

NUMBER_COLORS = {1: "blue", 2: "dark green", 3: "red", 4: "navy", 5: "brown", 6: "teal", 7: "black", 8: "gray"}


class MinesweeperApp:
    """Level buttons, a status line and a grid of labels, one per cell."""

    def __init__(self, root: tk.Tk) -> None:
        self.root = root
        root.title("Minesweeper")
        self.cell_font = tkfont.nametofont("TkDefaultFont").copy()
        self.cell_font.configure(size=12, weight="bold")

        top = tk.Frame(root)
        top.pack(padx=8, pady=8)
        for name in LEVELS:
            tk.Button(top, text=name, command=lambda level=name: self.new_game(level)).pack(side=tk.LEFT)

        self.info = tk.Label(root, font=self.cell_font)
        self.info.pack()
        self.message = tk.Label(root, font=self.cell_font)
        self.message.pack()
        self.board = tk.Frame(root)
        self.board.pack(padx=8, pady=8)

        self.game = None
        self.cells = []
        self.seconds = 0
        self.timer_job = None
        self.new_game("Beginner")

    def new_game(self, level: str) -> None:
        rows, cols, mines = LEVELS[level]
        self.game = Game(rows, cols, mines, random.Random())
        self.seconds = 0
        if self.timer_job is not None:
            self.root.after_cancel(self.timer_job)
        self.timer_job = self.root.after(1000, self.tick)

        for widget in self.board.winfo_children():
            widget.destroy()
        self.cells = [[self.make_cell(row, col) for col in range(cols)] for row in range(rows)]
        self.refresh()

    def make_cell(self, row: int, col: int) -> tk.Label:
        cell = tk.Label(self.board, width=2, height=1, font=self.cell_font, relief=tk.RAISED, borderwidth=2)
        cell.grid(row=row, column=col)
        cell.bind("<Button-1>", lambda event: self.reveal(row, col))
        cell.bind("<Button-3>", lambda event: self.flag(row, col))
        return cell

    def tick(self) -> None:
        if self.game.state != "playing":
            return
        self.seconds += 1
        self.refresh()
        self.timer_job = self.root.after(1000, self.tick)

    def reveal(self, row: int, col: int) -> None:
        self.game.reveal(row, col)
        self.refresh()

    def flag(self, row: int, col: int) -> None:
        self.game.toggle_flag(row, col)
        self.refresh()

    def refresh(self) -> None:
        game = self.game
        self.info.config(text=f"Mines left: {game.mines_left}     Time: {self.seconds} s")
        messages = {"won": "You won! Every safe cell is open.", "lost": "Boom! You hit a mine."}
        self.message.config(text=messages.get(game.state, ""))
        for row in range(game.rows):
            for col in range(game.cols):
                self.cells[row][col].config(**self.cell_look(row, col))

    def cell_look(self, row: int, col: int) -> dict:
        game = self.game
        if game.revealed[row][col]:
            if game.mine[row][col]:
                return {"text": "*", "fg": "black", "bg": "salmon", "relief": tk.SUNKEN}
            count = game.neighbors[row][col]
            text = str(count) if count else ""
            return {"text": text, "fg": NUMBER_COLORS.get(count, "black"), "bg": "white", "relief": tk.SUNKEN}
        if game.flagged[row][col]:
            return {"text": "F", "fg": "red", "bg": "lightgray", "relief": tk.RAISED}
        return {"text": "", "fg": "black", "bg": "lightgray", "relief": tk.RAISED}


def main() -> None:
    root = tk.Tk()
    MinesweeperApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
