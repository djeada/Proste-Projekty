import tkinter as tk

from projekty.python.fifteen_puzzle.src.gui.sytled_window import StyledWindow
from projekty.python.fifteen_puzzle.src.logic.puzzle_board import PuzzleBoard
from projekty.python.fifteen_puzzle.src.logic.utils import Position


class PuzzleGui:
    """
    Uses tkinter to display the board.
    Handles key presses.
    """

    def __init__(self, board: PuzzleBoard):
        """
        Initializes the GUI.
        """
        self.board = board
        self.root = StyledWindow()
        self.root.title("Fifteen Puzzle")
        self.root.resizable(False, False)
        self.root.bind("<Key>", self.key_pressed)
        self.canvas = tk.Canvas(self.root, width=410, height=500)
        self.canvas.pack()
        self.draw_board()

    def update_canvas(self) -> None:
        """
        Updates the canvas.
        """

        self.canvas.delete(tk.ALL)

        if self.board.is_solved():
            self.display_win_message()

        else:
            self.draw_board()

    def display_win_message(self) -> None:
        """
        Displays a message when the puzzle is solved.
        """
        self.canvas.create_text(200, 200, text="You win!", font=("Arial", 30))

    def draw_board(self) -> None:
        """
        Draws the board on the canvas.
        """
        for row in range(self.board.size):
            for col in range(self.board.size):
                x = col * 100 + 10
                y = row * 100 + 10
                background_color = (
                    "#90EE90"
                    if self.board.is_on_correct_position(Position(row, col))
                    else "#f0f0f0"
                )
                self.canvas.create_rectangle(
                    x, y, x + 90, y + 90, fill=background_color
                )
                self.canvas.create_text(
                    x + 45,
                    y + 45,
                    text=str(self.board.board[row][col]),
                    font=("Arial", 13),
                )
        self.canvas.create_text(
            200,
            450,
            text="Use the arrow keys to move the blank tile.",
            font=("Arial", 14),
        )

    def key_pressed(self, event: tk.Event) -> None:
        """
        Handles key presses.
        """
        if event.char == "q":
            self.root.destroy()
            exit()

        if self.board.is_solved():
            return
        print(event.char)
        if event.char == "w" or event.keysym == "Up":
            self.board.move_up()
        elif event.char == "s" or event.keysym == "Down":
            self.board.move_down()
        elif event.char == "a" or event.keysym == "Left":
            self.board.move_left()
        elif event.char == "d" or event.keysym == "Right":
            self.board.move_right()

        self.update_canvas()

    def run(self) -> None:
        """
        Starts the main loop of the GUI.
        """
        self.root.mainloop()
