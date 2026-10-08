"""Tkinter window for 2048: draws the board and turns key presses into moves."""
import tkinter as tk
import tkinter.font

from game_2048 import LOST, WON, Game

CELL = 100
GAP = 10
BOARD_BG = "#bbada0"
EMPTY_BG = "#cdc1b4"
TILE_COLORS = {  # value: (background, text)
    2: ("#eee4da", "#776e65"),
    4: ("#ede0c8", "#776e65"),
    8: ("#f2b179", "#f9f6f2"),
    16: ("#f59563", "#f9f6f2"),
    32: ("#f67c5f", "#f9f6f2"),
    64: ("#f65e3b", "#f9f6f2"),
    128: ("#edcf72", "#f9f6f2"),
    256: ("#edcc61", "#f9f6f2"),
    512: ("#edc850", "#f9f6f2"),
    1024: ("#edc53f", "#f9f6f2"),
    2048: ("#edc22e", "#f9f6f2"),
}
KEY_DIRECTIONS = {"up": "up", "w": "up", "down": "down", "s": "down",
                  "left": "left", "a": "left", "right": "right", "d": "right"}
MESSAGES = {
    WON: "You reached 2048! Press C to keep playing or R for a new game.",
    LOST: "Game over! Press R to play again.",
}


class App:
    def __init__(self, root):
        self.root = root
        root.title("2048")
        root.resizable(False, False)

        big = tkinter.font.nametofont("TkDefaultFont").copy()
        big.configure(size=16, weight="bold")
        self.tile_font = tkinter.font.nametofont("TkDefaultFont").copy()
        self.tile_font.configure(size=24, weight="bold")

        self.score_label = tk.Label(root, font=big)
        self.score_label.pack(pady=(10, 0))
        size = 2 * GAP + 4 * CELL + 3 * GAP
        self.canvas = tk.Canvas(root, width=size, height=size, bg=BOARD_BG, highlightthickness=0)
        self.canvas.pack(padx=10, pady=10)
        self.message_label = tk.Label(root, font=big)
        self.message_label.pack()
        tk.Button(root, text="New game", command=self.new_game).pack()
        tk.Label(root, text="Arrows or W A S D: move   R: restart   C: keep playing").pack(pady=(0, 10))

        root.bind("<Key>", self.on_key)
        self.new_game()

    def new_game(self):
        self.game = Game()
        self.draw()

    def on_key(self, event):
        key = event.keysym.lower()
        if key in KEY_DIRECTIONS:
            self.game.move(KEY_DIRECTIONS[key])
        elif key == "r":
            self.new_game()
            return
        elif key == "c":
            self.game.keep_playing = True
        self.draw()

    def draw(self):
        self.score_label.config(text=f"Score: {self.game.score}")
        self.message_label.config(text=MESSAGES.get(self.game.status(), ""))
        self.canvas.delete("all")
        for r, row in enumerate(self.game.board):
            for c, value in enumerate(row):
                x = GAP + c * (CELL + GAP)
                y = GAP + r * (CELL + GAP)
                background, text = TILE_COLORS.get(value, ("#3c3a32", "#f9f6f2")) if value else (EMPTY_BG, "")
                self.canvas.create_rectangle(x, y, x + CELL, y + CELL, fill=background, outline="")
                if value:
                    self.canvas.create_text(x + CELL / 2, y + CELL / 2, text=str(value), fill=text,
                                            font=self.tile_font)


def main():
    root = tk.Tk()
    App(root)
    root.mainloop()


if __name__ == "__main__":
    main()
