"""Tkinter window for the fifteen puzzle."""
import random
import tkinter
import tkinter.font

from fifteen_puzzle import SIZE, is_solved, move_gap, shuffle, slide, solved_board

ARROW_KEYS = {"Up": "up", "Down": "down", "Left": "left", "Right": "right"}


class PuzzleWindow:
    def __init__(self, root):
        self.root = root
        self.rng = random.Random()
        self.board = solved_board()
        self.moves = 0
        self.font = tkinter.font.nametofont("TkDefaultFont").copy()
        self.font.configure(size=18, weight="bold")

        root.title("Fifteen Puzzle")
        grid = tkinter.Frame(root, padx=16, pady=16)
        grid.pack()
        self.buttons = []
        for cell in range(SIZE * SIZE):
            button = tkinter.Button(
                grid,
                width=4,
                height=2,
                font=self.font,
                command=lambda c=cell: self.click(c),
            )
            button.grid(row=cell // SIZE, column=cell % SIZE, padx=3, pady=3)
            self.buttons.append(button)

        self.status = tkinter.Label(root, font=self.font)
        self.status.pack()
        tkinter.Button(root, text="New game", command=self.new_game).pack(pady=12)

        for key, direction in ARROW_KEYS.items():
            root.bind(f"<{key}>", lambda event, d=direction: self.arrow(d))
        self.new_game()

    def new_game(self):
        shuffle(self.board, self.rng)
        self.moves = 0
        self.refresh()

    def click(self, cell):
        if not is_solved(self.board) and slide(self.board, cell):
            self.moves += 1
        self.refresh()

    def arrow(self, direction):
        if not is_solved(self.board) and move_gap(self.board, direction):
            self.moves += 1
        self.refresh()

    def refresh(self):
        for cell, button in enumerate(self.buttons):
            tile = self.board[cell]
            button.config(text=str(tile) if tile else "")
        if is_solved(self.board):
            self.status.config(text=f"Solved in {self.moves} moves!")
        else:
            self.status.config(text=f"Moves: {self.moves}")


def main():
    root = tkinter.Tk()
    PuzzleWindow(root)
    root.mainloop()


if __name__ == "__main__":
    main()
