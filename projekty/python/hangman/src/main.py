"""
Game of hangman.
1. The word is chosen randomly from the list of words.
2. The player has to guess the word.
3. There are n squares drawn on the board, where n is the length of the word.
4. When the player guesses a letter, the squares with the letter are revealed.
5. If the player guesses the word, he wins.
6. If the player guesses a letter that is not in the word, an image from a list of images is drawn.

The board consits of an image on the left side and the squares representing the letters on the right side.
"""

import random
import sys
import tkinter as tk

words = ["python", "java", "kotlin", "javascript"]
paths = ["../resources/hangman_1.png", "../resources/hangman_2.png", "../resources/hangman_3.png", "../resources/hangman_4.png", "../resources/hangman_5.png", "../resources/hangman_6.png", "../resources/hangman_7.png"]


class Gui:
    """
    This class represents the GUI for the game.
    """

    def __init__(self, master):
        """
        Initializes the GUI.
        """
        self.master = master
        self.master.title("Hangman")
        self.master.geometry("800x500")
        self.master.configure(background="white")
        self.master.protocol("WM_DELETE_WINDOW", self.on_closing)

        frame = tk.Frame(self.master, bg="white")
        frame.pack()

        self.hangman_image = tk.PhotoImage(file=paths[0])
        self.hangman_label = tk.Label(frame, image=self.hangman_image, bg="white")
        self.hangman_label.pack(side=tk.LEFT)

        # canvas for guessed letters
        self.guessed_letters_canvas = tk.Canvas(frame, width=300, height=300, bg="white", bd=0, highlightthickness=0, relief='ridge')
        self.guessed_letters_canvas.pack(side=tk.RIGHT)

        # bottom frame for input and buttons
        bottom_frame = tk.Frame(self.master, bg="white")
        bottom_frame.pack(side=tk.RIGHT, padx=15)

        # input for guessed letters
        self.guessed_letters_label = tk.Label(bottom_frame, text="Input: ", bg="white")
        self.guessed_letters_label.pack(side=tk.LEFT)

        self.guessed_letters_entry = tk.Entry(bottom_frame)
        self.guessed_letters_entry.pack(side=tk.LEFT)

        # button for guess
        guess_button = tk.Button(bottom_frame, text="Guess", command=self.on_guess)
        guess_button.pack(side=tk.LEFT, padx=5)

        # button for restart
        restart_button = tk.Button(bottom_frame, text="Restart", command=self.on_restart)
        restart_button.pack(side=tk.LEFT, padx=5)

        self.word = random.choice(words)
        self.guessed_letters = ""
        self.hangman_image_index = 0
        self.draw_board()

    def draw_board(self):
        """
        Draws the board on the canvas.
        """
        # clear self.guessed_letters_canvas and draw n squares
        self.guessed_letters_canvas.delete("all")
        self.guessed_letters_canvas.create_text(120, 90, text="Guessed letters:", font=("Arial", 16))
        for i in range(len(self.word)):
            if self.word[i] in self.guessed_letters:
                self.guessed_letters_canvas.create_text(50 + i * 20, 150, text=self.word[i], font=("Arial", 16))
            else:
                self.guessed_letters_canvas.create_text(50 + i * 20, 150 , text="_", font=("Arial", 16))

    def on_guess(self):
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
        if self.hangman_image_index == len(paths):
            if set(self.word) == set(self.guessed_letters):
                self.display_winning_message()
            else:
                self.display_game_over()

    def display_winning_message(self):
        """
        Displays the winning message.
        """
        # remove all widgets from the root window
        for widget in self.master.winfo_children():
            widget.destroy()
        # create a new frame for the winning message
        frame = tk.Frame(self.master, bg="white")
        frame.pack()
        # create a label with the winning message
        winning_message = tk.Label(frame, text="You win!", bg="white")
        winning_message.pack()

    def display_game_over(self):
        """
        Displays the game over message.
        """
        # remove all widgets from the root window
        for widget in self.master.winfo_children():
            widget.destroy()
        # create a new frame for the game over message
        frame = tk.Frame(self.master, bg="white")
        frame.pack()
        # create a label with the game over message
        game_over_message = tk.Label(frame, text="You lose!", bg="white")
        game_over_message.pack()

    def update_hangman_image(self):
        """
        Updates the hangman image.
        """
        self.hangman_image = tk.PhotoImage(file=paths[self.hangman_image_index])
        self.hangman_label.configure(image=self.hangman_image)

    def on_restart(self):
        """
        Handles the event when the player wants to restart the game.
        """
        self.hangman_image_index = 0
        self.guessed_letters = ""
        self.word = random.choice(words)
        self.draw_board()
        self.update_hangman_image()

    def on_closing(self):
        """
        Handles the event when the player wants to quit the game.
        """
        self.master.destroy()
        sys.exit()

    def start(self):
        """
        Starts the GUI.
        """
        self.master.mainloop()


def main():
    """
    Starts the game.
    """
    root = tk.Tk()
    gui = Gui(root)
    gui.start()


if __name__ == "__main__":
    main()
