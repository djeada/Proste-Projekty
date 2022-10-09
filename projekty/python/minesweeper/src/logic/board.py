import random
from typing import List


class Board:
    """
    Holds the information about the cells including mines positions.
    """

    def __init__(self, height: int = 10, width: int = 10, num_mines: int = 20) -> None:
        self.board = [[0 for _ in range(width)] for _ in range(height)]
        self.num_mines = num_mines
        self.setup()

    def setup(self) -> None:
        """
        Fills the matrix with information about mines and numbers of mines in the neighbouring cells.
        """
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
        """
        Deletes the previous board and creates a new one.
        """
        width = len(self.board[0])
        height = len(self.board)
        self.board = [[0 for _ in range(width)] for _ in range(height)]
        self.setup()

    def count_neighbouring_mines(self, x: int, y: int) -> int:
        """
        Find the number of mines in the neighbouring cells.

        :param x: x coordinate of the cell
        :param y: y coordinate of the cell
        :return: number of mines in the neighbouring cells
        """
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
        """
        Constructs a list of all neighbouring cells that are not mines.

        :param x: x coordinate of the cell
        :param y: y coordinate of the cell
        :return: list of neighbouring cells that are not mines
        """

        free_cells = []

        def _get_all_neighbouring_free_cells(x: int, y: int) -> None:
            """
            Recursive function that finds all neighbouring cells that are not mines.

            :param x: x coordinate of the cell
            :param y: y coordinate of the cell
            """

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
                    if (
                        self.board[x + i][y + j] != -1
                        and (x + i, y + j) not in free_cells
                    ):
                        free_cells.append((x + i, y + j))
                        if self.board[x + i][y + j] == 0:
                            _get_all_neighbouring_free_cells(x + i, y + j)

        _get_all_neighbouring_free_cells(x, y)
        return free_cells

    def get_all_mines(self) -> List[List[int]]:
        """
        Constructs a list of positions of all mines.

        :return: list of positions of all mines
        """

        mines = []

        for x in range(len(self.board)):
            for y in range(len(self.board[0])):
                if self.board[x][y] == -1:
                    mines.append((x, y))

        return mines
