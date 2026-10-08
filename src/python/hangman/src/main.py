"""Hangman window: the gallows and the figure on the left, the word and the letter buttons on the right."""

import string
import tkinter as tk
import tkinter.font

from hangman import MAX_MISSES, Game, GameStatus, GuessResult, random_entry

GALLOWS = [(20, 230, 180, 230), (50, 230, 50, 20), (50, 20, 130, 20), (130, 20, 130, 50)]
# (stage, kind, coordinates): a body part is drawn once the number of misses reaches its stage
FIGURE = [
    (1, "oval", (110, 50, 150, 90)),
    (2, "line", (130, 90, 130, 150)),
    (3, "line", (130, 110, 100, 130)),
    (4, "line", (130, 110, 160, 130)),
    (5, "line", (130, 150, 105, 190)),
    (6, "line", (130, 150, 155, 190)),
]
MESSAGES = {
    GuessResult.HIT: "Good guess!",
    GuessResult.MISS: "Not in the word.",
    GuessResult.REPEAT: "You already guessed that letter.",
    GuessResult.INVALID: "Please enter a letter from a to z.",
}


def draw_figure(canvas: tk.Canvas, misses: int) -> None:
    canvas.delete("all")
    for coords in GALLOWS:
        canvas.create_line(*coords, width=5, capstyle=tk.ROUND)
    for stage, kind, coords in FIGURE:
        if misses >= stage:
            if kind == "oval":
                canvas.create_oval(*coords, width=5)
            else:
                canvas.create_line(*coords, width=5, capstyle=tk.ROUND)


class HangmanApp:
    def __init__(self, root: tk.Tk):
        self.root = root
        self.game = Game(random_entry())
        self.message = ""

        root.title("Hangman")
        big = tkinter.font.nametofont("TkDefaultFont").copy()
        big.configure(size=20, weight="bold")

        self.canvas = tk.Canvas(root, width=200, height=240, bg="white", highlightthickness=0)
        self.canvas.grid(row=0, column=0, rowspan=4, padx=16, pady=16)

        self.word_label = tk.Label(root, font=big)
        self.word_label.grid(row=0, column=1, sticky="w", padx=16, pady=(16, 4))
        self.info_label = tk.Label(root, justify="left", anchor="w")
        self.info_label.grid(row=1, column=1, sticky="w", padx=16)
        self.message_label = tk.Label(root, anchor="w")
        self.message_label.grid(row=2, column=1, sticky="w", padx=16, pady=4)

        keyboard = tk.Frame(root)
        keyboard.grid(row=3, column=1, sticky="w", padx=16)
        self.buttons = {}
        for index, letter in enumerate(string.ascii_lowercase):
            button = tk.Button(keyboard, text=letter.upper(), width=3,
                               command=lambda key=letter: self.guess(key))
            button.grid(row=index // 9, column=index % 9)
            self.buttons[letter] = button

        tk.Button(root, text="New game", command=self.new_game).grid(row=4, column=0, columnspan=2, pady=12)
        root.bind("<Key>", self.on_key)
        self.render()

    def new_game(self) -> None:
        self.game = Game(random_entry())
        self.message = ""
        self.render()

    def on_key(self, event: tk.Event) -> None:
        if len(event.char) == 1 and event.char.lower() in string.ascii_lowercase:
            self.guess(event.char.lower())

    def guess(self, letter: str) -> None:
        if self.game.status != GameStatus.PLAYING:
            return
        self.message = MESSAGES[self.game.guess(letter)]
        self.render()

    def render(self) -> None:
        game = self.game
        draw_figure(self.canvas, game.misses)
        self.word_label.config(text=game.masked())
        guessed = ", ".join(sorted(game.guessed)) or "-"
        self.info_label.config(text=f"Category: {game.category}\n"
                                    f"Wrong guesses left: {MAX_MISSES - game.misses}\n"
                                    f"Guessed: {guessed}")
        if game.status == GameStatus.WON:
            self.message = f"You won! The word was '{game.word}'."
        elif game.status == GameStatus.LOST:
            self.message = f"You lost. The word was '{game.word}'."
        self.message_label.config(text=self.message)
        for letter, button in self.buttons.items():
            button.config(state=tk.DISABLED if letter in game.guessed or game.status != GameStatus.PLAYING
                          else tk.NORMAL)


def main() -> None:
    root = tk.Tk()
    HangmanApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
