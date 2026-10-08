"""Battleship window: place your fleet on the left board, then fire at the computer's board on the right."""
import random
import tkinter as tk
import tkinter.font

from battleship import BOARD_SIZE, FLEET, INVALID, MISS, REPEAT, SUNK, Board, Computer

WATER = "#3a7ca5"
SHIP = "#adb5bd"
HIT_COLOR = "#e63946"
SUNK_COLOR = "#7f1d1d"
MISS_COLOR = "#e9ecef"
COLUMNS = "ABCDEFGHIJ"


def coordinate(x, y):
    return f"{COLUMNS[x]}{y + 1}"


def outcome(board, x, y, result):
    if result == SUNK:
        return f"sunk a ship of length {board.ships[board.ship_at[y][x]].length}"
    return "miss" if result == MISS else "hit"


def cell_look(board, x, y, show_ships):
    index = board.ship_at[y][x]
    if board.shot[y][x]:
        if index is None:
            return MISS_COLOR, "o"
        return (SUNK_COLOR, "#") if board.ships[index].sunk else (HIT_COLOR, "X")
    if show_ships and index is not None:
        return SHIP, ""
    return WATER, ""


class BattleshipApp:
    def __init__(self, root):
        self.rng = random.Random()
        self.horizontal = True
        self.message = ""
        self.status = tk.StringVar()
        self.cell_font = tkinter.font.nametofont("TkDefaultFont").copy()
        self.cell_font.configure(size=10, weight="bold")

        root.title("Battleship")
        boards = tk.Frame(root)
        boards.pack(padx=12, pady=(12, 6))
        self.own = self.make_grid(boards, "Your fleet", self.place_ship, 0)
        self.enemy = self.make_grid(boards, "Computer's waters", self.fire, 1)

        controls = tk.Frame(root)
        controls.pack(pady=6)
        self.rotate_button = tk.Button(controls, command=self.rotate, width=20)
        self.rotate_button.pack(side="left", padx=4)
        tk.Button(controls, text="Random fleet", command=self.random_fleet).pack(side="left", padx=4)
        tk.Button(controls, text="New game", command=self.new_game).pack(side="left", padx=4)

        tk.Label(root, textvariable=self.status, wraplength=560, justify="left").pack(padx=12, pady=(6, 12))
        self.new_game()

    def make_grid(self, parent, title, on_click, column):
        frame = tk.LabelFrame(parent, text=title, padx=6, pady=6)
        frame.grid(row=0, column=column, padx=6)
        buttons = [[None] * BOARD_SIZE for _ in range(BOARD_SIZE)]
        for y in range(BOARD_SIZE):
            for x in range(BOARD_SIZE):
                buttons[y][x] = tk.Button(frame, width=2, height=1, font=self.cell_font, padx=0, pady=0,
                                          command=lambda x=x, y=y: on_click(x, y))
                buttons[y][x].grid(row=y + 1, column=x)
        for x in range(BOARD_SIZE):
            tk.Label(frame, text=COLUMNS[x]).grid(row=0, column=x)
        for y in range(BOARD_SIZE):
            tk.Label(frame, text=str(y + 1)).grid(row=y + 1, column=BOARD_SIZE)
        return buttons

    def new_game(self):
        self.player = Board()
        self.computer_board = Board()
        self.computer_board.place_randomly(self.rng)
        self.computer = Computer()
        self.phase = "placing"
        self.message = f"Place your ships. Next ship: length {FLEET[0]}. Click a cell on your board."
        self.refresh()

    def next_ship(self):
        for index, ship in enumerate(self.player.ships):
            if not ship.placed:
                return index
        return None

    def rotate(self):
        self.horizontal = not self.horizontal
        self.refresh()

    def random_fleet(self):
        if self.phase != "placing":
            return
        self.player.place_randomly(self.rng)
        self.phase = "battle"
        self.message = "Battle! Click a cell on the computer's board to fire."
        self.refresh()

    def place_ship(self, x, y):
        if self.phase != "placing":
            return
        index = self.next_ship()
        if not self.player.place(index, x, y, self.horizontal):
            self.message = "Cannot place a ship there: it leaves the board or overlaps another ship."
        elif self.player.fleet_placed():
            self.phase = "battle"
            self.message = "All ships placed. Battle! Click a cell on the computer's board to fire."
        else:
            self.message = f"Ship placed. Next ship: length {FLEET[self.next_ship()]}."
        self.refresh()

    def fire(self, x, y):
        if self.phase != "battle":
            return
        result = self.computer_board.fire(x, y)
        if result in (REPEAT, INVALID):
            self.message = "You already fired at that cell. Choose another one."
            self.refresh()
            return
        text = f"You fire at {coordinate(x, y)}: {outcome(self.computer_board, x, y, result)}."
        if self.computer_board.all_sunk():
            self.phase = "over"
            self.message = text + " You won! All the computer's ships are sunk."
            self.refresh()
            return
        cx, cy = self.computer.choose(self.player, self.rng)
        result = self.player.fire(cx, cy)
        self.computer.report(self.player, cx, cy, result)
        text += f" Computer fires at {coordinate(cx, cy)}: {outcome(self.player, cx, cy, result)}."
        if self.player.all_sunk():
            self.phase = "over"
            text += " The computer sank your whole fleet. You lost."
        self.message = text
        self.refresh()

    def refresh(self):
        for y in range(BOARD_SIZE):
            for x in range(BOARD_SIZE):
                bg, text = cell_look(self.player, x, y, True)
                self.own[y][x].config(bg=bg, text=text)
                bg, text = cell_look(self.computer_board, x, y, self.phase == "over")
                self.enemy[y][x].config(bg=bg, text=text)
        self.rotate_button.config(text="Orientation: " + ("horizontal" if self.horizontal else "vertical"))
        self.status.set(self.message)


def main():
    root = tk.Tk()
    BattleshipApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
