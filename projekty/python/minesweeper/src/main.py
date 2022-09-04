from dataclasses import dataclass
import random
import tkinter as tk
from enum import Enum, auto
from pathlib import Path
from typing import List
from PIL import ImageTk, Image

'''
Minesweeper game implemented using Tkinter.
'''


class Board:
    '''	
    Holds the information about the cells including mines positions.
    '''

    def __init__(self, height: int = 10, width: int = 10, num_mines: int = 20) -> None:
        self.board = [[0 for _ in range(width)] for _ in range(height)]
        self.num_mines = num_mines
        self.setup()

    def setup(self) -> None:
        '''
        Fills the matrix with information about mines and numbers of mines in the neighbouring cells.
        '''
        width = len(self.board[0])
        height = len(self.board)

        for _ in range(self.num_mines):
            x = random.randint(0, height - 1)
            y = random.randint(0, width - 1)

            current_cell = self.board[x][y]
            while current_cell != 0:
                x = random.randint(0, height - 1)
                y = random.randint(0, width - 1)
                current_cell = self.board[x][y]

            self.board[x][y] = -1

        for x in range(height):
            for y in range(width):
                if self.board[x][y] == -1:
                    continue
                else:
                    self.board[x][y] = self.count_neighbouring_mines(x, y)

    def reset(self) -> None:
        '''
        Deletes the previous board and creates a new one.
        '''
        width = len(self.board[0])
        height = len(self.board)
        self.board = [[0 for _ in range(width)] for _ in range(height)]
        self.setup()

    def count_neighbouring_mines(self, x: int, y: int) -> int:
        '''
        Find the number of mines in the neighbouring cells.

        :param x: x coordinate of the cell
        :param y: y coordinate of the cell
        :return: number of mines in the neighbouring cells
        '''
        count = 0
        for i in range(-1, 2):
            for j in range(-1, 2):
                if i == 0 and j == 0:
                    continue
                if x + i < 0 or x + i >= len(self.board):
                    continue
                if y + j < 0 or y + j >= len(self.board[0]):
                    continue
                if self.board[x + i][y + j] == -1:
                    count += 1
        return count

    def get_all_neighbouring_free_cells(self, x: int, y: int) -> List[List[int]]:
        '''
        Constructs a list of all neighbouring cells that are not mines.

        :param x: x coordinate of the cell
        :param y: y coordinate of the cell
        :return: list of neighbouring cells that are not mines
        '''

        free_cells = []

        def _get_all_neighbouring_free_cells(x: int, y: int) -> None:
            '''
            Recursive function that finds all neighbouring cells that are not mines.
            
            :param x: x coordinate of the cell
            :param y: y coordinate of the cell
            '''

            if self.board[x][y] == -1:
                return

            # free cells have value other than -1
            for i in range(-1, 2):
                for j in range(-1, 2):
                    if i == 0 and j == 0:
                        continue
                    if x + i < 0 or x + i >= len(self.board):
                        continue
                    if y + j < 0 or y + j >= len(self.board[0]):
                        continue
                    if self.board[x + i][y + j] != -1 and (x + i, y + j) not in free_cells:
                        free_cells.append((x + i, y + j))
                        if self.board[x + i][y + j] == 0:
                            _get_all_neighbouring_free_cells(x + i, y + j)

        _get_all_neighbouring_free_cells(x, y)
        return free_cells

    def get_all_mines(self) -> List[List[int]]:
        '''
        Constructs a list of positions of all mines.

        :return: list of positions of all mines
        '''

        mines = []

        for x in range(len(self.board)):
            for y in range(len(self.board[0])):
                if self.board[x][y] == -1:
                    mines.append((x, y))

        return mines

class GameState(Enum):
    '''
    Enum that holds the states of the game.
    '''
    default = auto()
    win = auto()
    loss = auto()


class ButtonState(Enum):
    '''
    Enum that holds the states of the buttons.
    '''
    default = auto()
    revealed = auto()
    flagged = auto()


@dataclass
class Button:
    '''
    Holds the information about the widget and the current state of the button.
    '''
    widget: tk.Button
    state: ButtonState


class Gui:
    '''
    Main window with all the game logic.
    '''

    def __init__(self, root: tk.Tk, board: Board) -> None:
        self.root = root
        self.board = board
        self.state = GameState.default
        self.flagged_mines_counter = 0
        self.buttons = [[None for _ in range(len(self.board.board[0]))] for _ in range(len(self.board.board))]
        self.setup()

    def update_smiley(self) -> None:
        '''
        Updates the smiley image.
        '''

        state_to_path = {
            GameState.default: Path("../resources/happy_smiley.png"),
            GameState.win:Path("../resources/sunglasses_smiley.png"),
            GameState.loss: Path("../resources/sad_smiley.png"),
        }

        path = state_to_path[self.state]
        self.smiley_image = ImageTk.PhotoImage(Image.open(path))
        self.smiley_button.config(image=self.smiley_image)

    def setup(self) -> None:
        '''
        Creates the frames and internal widgets.
        '''
        frame1 = tk.Frame(self.root)
        frame1.pack(fill=tk.X, padx=5, pady=5)

        self.draw_panel(frame1)

        frame2 = tk.Frame(self.root)
        frame2.pack(fill=tk.X, padx=5, pady=5)

        self.draw_board(frame2)

        self.state = GameState.default
        self.update_smiley()

    def draw_panel(self, window: tk.Frame) -> None:
        '''
        Draws the panel with the smiley button, a timer and 
        a label displaying the number of flagged mines.

        :param window: frame to draw the panel in
        '''

        mines_label = tk.Label(window, text="Mines: ")
        mines_label.grid(row=0, column=0, padx=10)

        self.mines_label = tk.Label(window, text=str(self.board.num_mines), bg="black", fg="red",
                                    font=("Digital-7", 20))
        self.mines_label.grid(row=0, column=1)

        self.smiley_button = tk.Button(window, command=self.restart)
        self.smiley_button.photo = ImageTk.PhotoImage(Image.open(Path("../resources/happy_smiley.png")).resize((39, 50)))
        self.smiley_button.config(image=self.smiley_button.photo)
        self.smiley_button.grid(row=0, column=3, padx=100)

        time_label = tk.Label(window, text="Time: ")
        time_label.grid(row=0, column=5)

        self.time_label = tk.Label(window, text="0:0", bg="black", fg="red", font=("Digital-7", 20))
        self.time_label.grid(row=0, column=6)
        self.time_label.after(1000, self.update_time)

    def update_time(self) -> None:
        '''
        Updates the time label.
        '''
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
        '''
        Restarts the game.
        '''
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
                button = tk.Button(window, width=5, height=3, command=lambda x=x, y=y: self.reveal(x, y))
                # add right click event
                button.bind("<Button-3>", lambda event, x=x, y=y: self.handle_flag_request(x, y))
                button.grid(row=x, column=y)
                self.buttons[x][y] = Button(widget=button, state=ButtonState.default)

    def handle_flag_request(self, x: int, y: int) -> None:
        '''
        Handles the right click event on a board button.
        If the button is flagged, it will be unflagged.
        If the button is not flagged, it will be flagged.

        :param x: x position of the button
        :param y: y position of the button
        '''
        state = self.buttons[x][y].state
        button = self.buttons[x][y].widget

        if state == ButtonState.default:
            button.photo = ImageTk.PhotoImage(Image.open(Path("../resources/flag.png")).resize((39, 50)))
            button.config(image=button.photo)
            button.config(width=0, height=0)
            self.buttons[x][y].state = ButtonState.flagged
            self.flagged_mines_counter += 1
        elif state == ButtonState.flagged:
            button.photo = None
            button.config(image='')
            button.config(width=5, height=3)
            self.buttons[x][y].state = ButtonState.default
            self.flagged_mines_counter -= 1

        self.mines_label.config(text=str(self.board.num_mines - self.flagged_mines_counter))
        self.check_for_win()

    def reveal(self, x: int, y: int) -> None:
        '''
        Reveals a value hidden behind the button.
        
        - If the value is a mine, the game is over.
        - If the value is 0, all the surrounding buttons will be revealed.
        - If the value is not 0, the button will be revealed.

        :param x: x position of the button
        :param y: y position of the button
        '''
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
            button.config(command=None, relief='sunken', background='darkGray')

            if value != 0:
                button.photo = ImageTk.PhotoImage(Image.open(Path(f"../resources../{value}.png")).resize((39, 50)))
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
        '''
        Handles the game over situation.
        
        - Shows all the mines on the board.
        - Changes the smiley button to a sad one.
        - Disables all the buttons.
        '''

        for mine_cell in self.board.get_all_mines():
            mine_x = mine_cell[0]
            mine_y = mine_cell[1]
            mine_button = self.buttons[mine_x][mine_y].widget
            mine_button.config(command=None, relief='sunken', background='red')
            mine_button.config(width=0, height=0)
            mine_button.photo = ImageTk.PhotoImage(Image.open(Path("../resources/mine.png")).resize((39, 50)))
            mine_button.config(image=mine_button.photo)

        # remove command from buttons
        for row in self.buttons:
            for button in row:
                button.widget.config(command=None, relief='sunken')
                button.state = ButtonState.revealed

        # update state
        self.state = GameState.loss
        self.update_smiley()

    def check_for_win(self) -> None:
        '''
        Checks if the game is won.

        - If the user has flagged all the mines, he wins.
        - If the game is won, the smiley button will be changed to a smiley face.
        '''
        for mine_cell in self.board.get_all_mines():
            mine_x = mine_cell[0]
            mine_y = mine_cell[1]
            state = self.buttons[mine_x][mine_y].state
            if state != ButtonState.flagged:
                return

        self.state = GameState.win
        self.update_smiley()

def main() -> None:
    root = tk.Tk()
    board = Board()
    gui = Gui(root, board)
    root.mainloop()


if __name__ == '__main__':
    main()
