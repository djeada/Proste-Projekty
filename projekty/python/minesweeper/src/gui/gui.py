import tkinter as tk
from enum import Enum, auto
from pathlib import Path
from tkinter import ttk

from PIL import ImageTk, Image

from projekty.python.minesweeper.src.gui.sytled_window import StyledWindow
from projekty.python.minesweeper.src.logic.board import Board
from projekty.python.minesweeper.src.utils.consts import Consts
from projekty.python.minesweeper.src.utils.utils import (
    GameState,
    ButtonState,
    ButtonInfo,
)


class Gui:
    """
    Main window with all the game logic.
    """

    def __init__(self, board: Board) -> None:
        self.root = StyledWindow()
        self.root.title("Minesweeper")
        self.board = board
        self.state = GameState.default
        self.flagged_mines_counter = 0
        self.buttons = [
            [None for _ in range(len(self.board.board[0]))]
            for _ in range(len(self.board.board))
        ]
        self.setup()

    def run(self) -> None:
        """
        Starts the main loop of the GUI.
        """
        self.root.mainloop()

    def update_smiley(self) -> None:
        """
        Updates the smiley image.
        """

        state_to_path = {
            GameState.default: Path("../resources/happy_smiley.png"),
            GameState.win: Path("../resources/sunglasses_smiley.png"),
            GameState.loss: Path("../resources/sad_smiley.png"),
        }

        path = state_to_path[self.state]
        self.smiley_image = ImageTk.PhotoImage(Image.open(path))
        self.smiley_button.config(image=self.smiley_image)

    def setup(self) -> None:
        """
        Creates the frames and internal widgets.
        """
        frame1 = ttk.Frame(self.root)
        frame1.pack(fill=tk.X, padx=5, pady=5)

        self.draw_panel(frame1)

        frame2 = ttk.Frame(self.root)
        frame2.pack(fill=tk.X, padx=5, pady=5)

        self.draw_board(frame2)

        self.state = GameState.default
        self.update_smiley()

    def draw_panel(self, window: ttk.Frame) -> None:
        """
        Draws the panel with the smiley button, a timer and
        a label displaying the number of flagged mines.

        :param window: frame to draw the panel in
        """

        mines_label = ttk.Label(window, text="Mines: ")
        mines_label.grid(row=0, column=0, padx=10)

        self.mines_label = ttk.Label(
            window,
            text=str(self.board.num_mines),
            font=("Digital-7", 20),
        )
        self.mines_label.grid(row=0, column=1)

        self.smiley_button = ttk.Button(window, command=self.restart)
        self.smiley_button.photo = ImageTk.PhotoImage(
            Image.open(Path(Consts.HappySmileyPath)).resize((39, 50))
        )
        self.smiley_button.config(image=self.smiley_button.photo)
        self.smiley_button.grid(row=0, column=3, padx=100)

        time_label = ttk.Label(window, text="Time: ")
        time_label.grid(row=0, column=5)

        self.time_label = ttk.Label(window, text="0:0", font=("Digital-7", 20))
        self.time_label.grid(row=0, column=6)
        self.time_label.after(1000, self.update_time)

    def update_time(self) -> None:
        """
        Updates the time label.
        """
        time_text = self.time_label["text"]

        seconds = int(time_text.split(":")[1])
        minutes = int(time_text.split(":")[0])

        seconds += 1
        if seconds == 60:
            seconds = 0
            minutes += 1

        time_text = f"{minutes}:{seconds}"

        self.time_label.config(text=str(time_text))
        self.time_label.after(1000, self.update_time)

    def restart(self) -> None:
        """
        Restarts the game.
        """
        # remove all widgets from root
        for child in self.root.winfo_children():
            child.destroy()
        self.board.reset()
        self.flagged_mines_counter = 0
        self.setup()

    def draw_board(self, window) -> None:
        # draw board using tkinter buttons
        for x in range(len(self.board.board)):
            for y in range(len(self.board.board[0])):
                button = tk.Button(
                    window,
                    width=3,
                    height=3,
                    bg="white",
                    command=lambda x=x, y=y: self.reveal(x, y),
                )
                # add right click event
                button.bind(
                    "<Button-3>", lambda event, x=x, y=y: self.handle_flag_request(x, y)
                )
                button.grid(row=x, column=y)
                self.buttons[x][y] = ButtonInfo(
                    widget=button, state=ButtonState.default
                )

    def handle_flag_request(self, x: int, y: int) -> None:
        """
        Handles the right click event on a board button.
        If the button is flagged, it will be unflagged.
        If the button is not flagged, it will be flagged.

        :param x: x position of the button
        :param y: y position of the button
        """
        state = self.buttons[x][y].state
        button = self.buttons[x][y].widget

        if state == ButtonState.default:
            button.photo = ImageTk.PhotoImage(
                Image.open(Path(Consts.FlagPath)).resize((39, 50))
            )
            button.config(image=button.photo)
            button.config(width=0, height=0)
            self.buttons[x][y].state = ButtonState.flagged
            self.flagged_mines_counter += 1
        elif state == ButtonState.flagged:
            button.photo = None
            button.config(image="")
            button.config(width=5, height=3)
            self.buttons[x][y].state = ButtonState.default
            self.flagged_mines_counter -= 1

        self.mines_label.config(
            text=str(self.board.num_mines - self.flagged_mines_counter)
        )
        self.check_for_win()

    def reveal(self, x: int, y: int) -> None:
        """
        Reveals a value hidden behind the button.

        - If the value is a mine, the game is over.
        - If the value is 0, all the surrounding buttons will be revealed.
        - If the value is not 0, the button will be revealed.

        :param x: x position of the button
        :param y: y position of the button
        """
        # reveal the cell on a given button
        button = self.buttons[x][y].widget
        state = self.buttons[x][y].state

        if state != ButtonState.default:
            return

        value = self.board.board[x][y]

        if value == -1:
            self.game_over()
            return

        def _update_button(button, value):
            button.config(command=None, relief="sunken")

            if value != 0:
                button.photo = ImageTk.PhotoImage(
                    Image.open(Path(Consts.DigitPaths[value])).resize((39, 50))
                )
                button.config(image=button.photo)
                button.config(width=0, height=0)

        self.buttons[x][y].state = ButtonState.revealed
        _update_button(button, value)

        if value == 0:
            for free_cell in self.board.get_all_neighbouring_free_cells(x, y):
                neighbour_x = free_cell[0]
                neighbour_y = free_cell[1]
                neighbour_value = self.board.board[neighbour_x][neighbour_y]
                neighbour_button = self.buttons[neighbour_x][neighbour_y].widget
                self.buttons[neighbour_x][neighbour_y].state = ButtonState.revealed
                _update_button(neighbour_button, neighbour_value)

    def game_over(self) -> None:
        """
        Handles the game over situation.

        - Shows all the mines on the board.
        - Changes the smiley button to a sad one.
        - Disables all the buttons.
        """

        for mine_cell in self.board.get_all_mines():
            mine_x = mine_cell[0]
            mine_y = mine_cell[1]
            mine_button = self.buttons[mine_x][mine_y].widget
            mine_button.config(command=None, relief="sunken")
            mine_button.config(width=0, height=0)
            mine_button.photo = ImageTk.PhotoImage(
                Image.open(Path(Consts.MinePath)).resize((39, 50))
            )
            mine_button.config(image=mine_button.photo)

        # remove command from buttons
        for row in self.buttons:
            for button in row:
                button.widget.config(command=None, relief="sunken")
                button.state = ButtonState.revealed

        # update state
        self.state = GameState.loss
        self.update_smiley()

    def check_for_win(self) -> None:
        """
        Checks if the game is won.

        - If the user has flagged all the mines, he wins.
        - If the game is won, the smiley button will be changed to a smiley face.
        """
        for mine_cell in self.board.get_all_mines():
            mine_x = mine_cell[0]
            mine_y = mine_cell[1]
            state = self.buttons[mine_x][mine_y].state
            if state != ButtonState.flagged:
                return

        self.state = GameState.win
        self.update_smiley()
