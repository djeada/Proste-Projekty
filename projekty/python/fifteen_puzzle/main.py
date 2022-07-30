'''
Fifteen puzzle is a game where you are given a board and you need to move all the 
tiles to the empty space to get the board in order.
At the start of the game, you are given a board with some tiles in random order.
There is only one empty space on the board.
You can move the tiles to the empty space by swapping them.
Once all the tiles are in order, you win the game.
'''

import random
import tkinter
from dataclasses import dataclass


@dataclass
class Position:
    '''
    Representation of a position on the board.
    '''
    x: int
    y: int

    def __add__(self, other):
        return Position(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Position(self.x - other.x, self.y - other.y)

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __hash__(self):
        return hash((self.x, self.y))

    def __repr__(self):
        return f'({self.x}, {self.y})'

    def __str__(self):
        return f'({self.x}, {self.y})'


class PuzzleBoard:
    '''
    This class represents a board for the Fifteen Puzzle.
    '''

    def __init__(self):
        '''
        Initializes a board with a 2D list of tiles.
        '''
        self.size = 4
        self.board = [[i + self.size * j for i in range(1, self.size + 1)] for j in range(self.size)]
        self.board[self.size - 1][self.size - 1] = ' '
        # shuffle all the tiles on the board
        # tiles can also switch rows
        for _ in range(self.size * self.size):
            pos_a = Position(random.randint(0, self.size - 1), random.randint(0, self.size - 1))
            pos_b = Position(random.randint(0, self.size - 1), random.randint(0, self.size - 1))
            self.swap_positions(pos_a, pos_b)

    def swap_positions(self, pos_a: Position, pos_b: Position) -> None:
        '''
        Swaps the tiles at the given positions.
        '''
        self.board[pos_a.x][pos_a.y], self.board[pos_b.x][pos_b.y] = self.board[pos_b.x][pos_b.y], self.board[pos_a.x][
            pos_a.y]

    def blank_position(self):
        '''
        Returns the position of the blank tile.
        '''
        for row in range(self.size):
            for col in range(self.size):
                if self.board[row][col] == ' ':
                    return Position(row, col)

        raise Exception('No blank tile found.')

    def can_be_moved(self, position: Position) -> bool:
        '''
        Returns True if a given tile can be moved and False otherwise.
        '''
        return self.are_adjacent(self.blank_position(), position)

    def are_adjacent(self, pos_a: Position, pos_b: Position) -> bool:
        '''
        Returns True if the tiles at the given positions are adjacent and False otherwise.
        '''
        return abs(pos_a.x - pos_b.x) + abs(pos_a.y - pos_b.y) == 1

    def move(self, position: Position) -> None:
        '''
        Moves the tile at the given position to the blank space.
        '''
        if not self.can_be_moved(position):
            raise Exception('Cannot move a given tile.')

        self.swap_positions(self.blank_position(), position)

    def move_up(self):
        '''
        Moves the blank tile up.
        '''
        pos = self.blank_position() + Position(1, 0)
        if pos.x >= self.size:
            return
        self.move(pos)

    def move_down(self):
        '''
        Moves the blank tile down.
        '''
        pos = self.blank_position() + Position(-1, 0)
        if pos.x < 0:
            return
        self.move(pos)

    def move_left(self):
        '''
        Moves the blank tile left.
        '''
        pos = self.blank_position() + Position(0, 1)
        if pos.y >= self.size:
            return
        self.move(pos)

    def move_right(self):
        '''
        Moves the blank tile right.
        '''
        pos = self.blank_position() + Position(0, -1)
        if pos.y < 0:
            return
        self.move(pos)

    def is_on_correct_position(self, position: Position) -> bool:
        '''
        Returns True if the tile at the given position is in the correct position and False otherwise.
        '''
        return self.board[position.x][position.y] == position.y + self.size * position.x + 1

    def is_solved(self) -> bool:
        '''
        Returns True if the board is solved and False otherwise.
        '''
        for row in range(self.size):
            for col in range(self.size):
                if not self.is_on_correct_position(Position(row, col)):
                    return False

        return True

    def __str__(self):
        '''
        Returns a string representation of the board.
        '''
        result = ''
        for row in self.board:
            for elem in row:
                elem = str(elem)
                if len(elem) == 1:
                    elem = elem + ' '
                result += elem + ' '

            result += '\n'

        return result


class PuzzleGui:
    '''
    Use Tkinter to display the board.
    Handles the user input from the keyboard.
    '''

    def __init__(self, board):
        '''
        Initializes the GUI.
        '''
        self.board = board
        self.window = tkinter.Tk()
        self.window.title('Fifteen Puzzle')
        self.window.resizable(False, False)
        self.window.bind('<Key>', self.key_pressed)
        self.canvas = tkinter.Canvas(self.window, width=400, height=500)
        self.canvas.pack()
        self.draw_board()

    def update_canvas(self):
        '''
        Updates the canvas.
        '''

        self.canvas.delete(tkinter.ALL)

        if self.board.is_solved():
            self.display_win_message()

        else:
            self.draw_board()

    def display_win_message(self):
        '''
        Displays a message when the puzzle is solved.
        '''
        self.canvas.create_text(200, 200, text='You win!', font=('Arial', 30))

    def draw_board(self):
        '''
        Draws the board on the canvas.
        '''
        for row in range(self.board.size):
            for col in range(self.board.size):
                x = col * 100 + 10
                y = row * 100 + 10
                background_color = '#90EE90' if self.board.is_on_correct_position(Position(row, col)) else '#f0f0f0'
                self.canvas.create_rectangle(x, y, x + 90, y + 90, fill=background_color)
                self.canvas.create_text(x + 45, y + 45, text=str(self.board.board[row][col]), font=('Arial', 13))
        self.canvas.create_text(200, 450, text='Use the arrow keys to move the blank tile.', font=('Arial', 14))

    def key_pressed(self, event):
        '''
        Handles key presses.
        '''
        if event.char == 'q':
            self.window.destroy()
            exit()

        if self.board.is_solved():
            return
        print(event.char)
        if event.char == 'w' or event.keysym == 'Up':
            self.board.move_up()
        elif event.char == 's' or event.keysym == 'Down':
            self.board.move_down()
        elif event.char == 'a' or event.keysym == 'Left':
            self.board.move_left()
        elif event.char == 'd' or event.keysym == 'Right':
            self.board.move_right()

        self.update_canvas()

    def run(self):
        '''
        Runs the GUI.
        '''
        self.window.mainloop()


puzzle_board = PuzzleBoard()
gui = PuzzleGui(puzzle_board)
gui.run()
