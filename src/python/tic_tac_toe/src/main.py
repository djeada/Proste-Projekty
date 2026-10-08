"""Tkinter window for tic-tac-toe: nine cells, a score and a mode switch."""

import tkinter as tk
import tkinter.font

from tic_tac_toe import best_move, is_over, legal_moves, new_board, other_mark, play, winner, winning_line


class App:
    def __init__(self, root):
        self.root = root
        self.vs_computer = False
        self.scores = {"X": 0, "O": 0, "draw": 0}
        root.title("Tic-Tac-Toe")

        cell_font = tkinter.font.nametofont("TkDefaultFont").copy()
        cell_font.configure(size=20, weight="bold")

        self.cells = []
        grid = tk.Frame(root)
        grid.pack(padx=20, pady=10)
        for cell in range(9):
            button = tk.Button(grid, width=4, height=2, font=cell_font,
                               command=lambda number=cell: self.on_click(number))
            button.grid(row=cell // 3, column=cell % 3, padx=3, pady=3)
            self.cells.append(button)

        self.status = tk.Label(root)
        self.status.pack()
        self.score = tk.Label(root)
        self.score.pack(pady=(8, 0))

        controls = tk.Frame(root)
        controls.pack(pady=10)
        tk.Button(controls, text="New round", command=self.new_round).pack(side="left", padx=4)
        self.mode_button = tk.Button(controls, command=self.toggle_mode)
        self.mode_button.pack(side="left", padx=4)
        tk.Button(controls, text="Quit", command=root.destroy).pack(side="left", padx=4)

        self.new_round()

    def new_round(self):
        self.board = new_board()
        self.turn = "X"
        self.draw()

    def toggle_mode(self):
        self.vs_computer = not self.vs_computer
        self.new_round()

    def on_click(self, cell):
        if is_over(self.board) or cell not in legal_moves(self.board):
            return
        self.place(cell)
        if self.vs_computer and not is_over(self.board):
            self.place(best_move(self.board, self.turn))
        self.draw()

    def place(self, cell):
        """Place the mark of the player whose turn it is and count the result when the round ends."""
        self.board = play(self.board, cell, self.turn)
        if is_over(self.board):
            self.scores[winner(self.board) or "draw"] += 1
        else:
            self.turn = other_mark(self.turn)

    def draw(self):
        line = winning_line(self.board) or ()
        for cell, button in enumerate(self.cells):
            mark = self.board[cell]
            button.config(text=mark if mark != " " else str(cell + 1),
                          bg="#ffd966" if cell in line else self.root.cget("bg"))
        if winner(self.board):
            self.status.config(text=f"{winner(self.board)} wins! Press New round to play again.")
        elif is_over(self.board):
            self.status.config(text="It is a draw! Press New round to play again.")
        else:
            self.status.config(text=f"Turn: {self.turn}")
        self.mode_button.config(text="Mode: vs computer" if self.vs_computer else "Mode: two players")
        self.score.config(text=f"X wins: {self.scores['X']}    O wins: {self.scores['O']}    "
                               f"Draws: {self.scores['draw']}")


def main():
    root = tk.Tk()
    App(root)
    root.mainloop()


if __name__ == "__main__":
    main()
