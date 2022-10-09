import random
import sys
import tkinter as tk
from tkinter import ttk

from projekty.python.hangman.src.gui.sytled_window import StyledWindow
from projekty.python.hangman.src.logic.config import Config


class Gui:
    """
    This class represents the GUI for the game.
    """

    def __init__(self, config: Config):
        self.config = config
        self.root = StyledWindow()
        self.root.title("Hangman")
        self.root.geometry("800x500")
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)

        frame = ttk.Frame(self.root)
        frame.pack()

        self.hangman_image = tk.PhotoImage(file=self.config.paths[0])
        self.hangman_label = ttk.Label(frame, image=self.hangman_image)
        self.hangman_label.pack(side=tk.LEFT)

        # canvas for guessed letters
        self.guessed_letters_canvas = tk.Canvas(
            frame,
            width=300,
            height=300,
            bd=0,
            highlightthickness=0,
            relief="ridge",
            bg="#fafafa",
        )
        self.guessed_letters_canvas.pack(side=tk.RIGHT)

        # bottom frame for input and buttons
        bottom_frame = ttk.Frame(self.root)
        bottom_frame.pack(side=tk.RIGHT, padx=15)

        # input for guessed letters
        self.guessed_letters_label = ttk.Label(bottom_frame, text="Input: ")
        self.guessed_letters_label.pack(side=tk.LEFT)

        self.guessed_letters_entry = ttk.Entry(bottom_frame)
        self.guessed_letters_entry.pack(side=tk.LEFT)

        # button for guess
        guess_button = ttk.Button(bottom_frame, text="Guess", command=self.on_guess)
        guess_button.pack(side=tk.LEFT, padx=5)

        # button for restart
        restart_button = ttk.Button(
            bottom_frame, text="Restart", command=self.on_restart
        )
        restart_button.pack(side=tk.LEFT, padx=5)

        self.word = random.choice(self.config.words)
        self.guessed_letters = ""
        self.hangman_image_index = 0
        self.draw_board()

    def draw_board(self) -> None:
        """
        Draws the board on the canvas.
        """
        # clear self.guessed_letters_canvas and draw n squares
        self.guessed_letters_canvas.delete("all")
        self.guessed_letters_canvas.create_text(
            120, 90, text="Guessed letters:", font=("Arial", 16)
        )
        for i in range(len(self.word)):
            if self.word[i] in self.guessed_letters:
                self.guessed_letters_canvas.create_text(
                    50 + i * 20, 150, text=self.word[i], font=("Arial", 16)
                )
            else:
                self.guessed_letters_canvas.create_text(
                    50 + i * 20, 150, text="_", font=("Arial", 16)
                )

    def on_guess(self) -> None:
        """
        Handles the event when the player guesses a letter.
        """
        new_guess = self.guessed_letters_entry.get()
        self.guessed_letters += new_guess
        self.guessed_letters_entry.delete(0, tk.END)
        self.draw_board()
        if new_guess not in self.word:
            self.hangman_image_index += 1
            self.update_hangman_image()
        if self.hangman_image_index == len(self.config.paths):
            if set(self.word) == set(self.guessed_letters):
                self.display_winning_message()
            else:
                self.display_game_over()

    def display_winning_message(self) -> None:
        """
        Displays the winning message.
        """
        # remove all widgets from the root window
        for widget in self.root.winfo_children():
            widget.destroy()
        # create a new frame for the winning message
        frame = ttk.Frame(self.root)
        frame.pack()
        # create a label with the winning message
        winning_message = ttk.Label(frame, text="You win!")
        winning_message.pack()

    def display_game_over(self) -> None:
        """
        Displays the game over message.
        """
        # remove all widgets from the root window
        for widget in self.root.winfo_children():
            widget.destroy()
        # create a new frame for the game over message
        frame = ttk.Frame(self.root)
        frame.pack()
        # create a label with the game over message
        game_over_message = ttk.Label(frame, text="You lose!")
        game_over_message.pack()

    def update_hangman_image(self) -> None:
        """
        Updates the hangman image.
        """
        self.hangman_image = tk.PhotoImage(
            file=self.config.paths[self.hangman_image_index]
        )
        self.hangman_label.configure(image=self.hangman_image)

    def on_restart(self) -> None:
        """
        Handles the event when the player wants to restart the game.
        """
        self.hangman_image_index = 0
        self.guessed_letters = ""
        self.word = random.choice(self.config.words)
        self.draw_board()
        self.update_hangman_image()

    def on_closing(self) -> None:
        """
        Handles the event when the player wants to quit the game.
        """
        self.root.destroy()
        sys.exit()

    def run(self) -> None:
        """
        Starts the main loop of the GUI.
        """
        self.root.mainloop()
