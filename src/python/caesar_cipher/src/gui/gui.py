import tkinter as tk
from enum import Enum, auto
from tkinter import ttk

from src.gui.sytled_window import StyledWindow
from src.logic.caesar_cipher import caesar_encrypt, caesar_decrypt


class CipherMode(Enum):
    """
    Possible cipher modes.
    """

    CIPHER = auto()
    DECIPHER = auto()


class Gui:
    """
    Application window and its widgets.
    """

    def __init__(self):
        self.root = StyledWindow()
        self.root.title("Caesar Cipher")
        self.root.geometry("400x350")

        self._cipher_decipher_var = tk.StringVar()
        self._cipher_decipher_var.set(CipherMode.CIPHER.name)

        frame1 = ttk.Frame(self.root)
        frame1.pack(fill=tk.X, padx=5, pady=5)

        self.message_label = ttk.Label(frame1, text="Message:", font=("Arial", 12))
        self.message_label.pack(side=tk.LEFT)

        self.message_entry = ttk.Entry(frame1, width=20, font=("Arial", 12))
        self.message_entry.pack(fill=tk.X, padx=15, expand=True)

        frame2 = ttk.Frame(self.root)
        frame2.pack(fill=tk.X, padx=5, pady=5)

        self.key_label = ttk.Label(frame2, text="Key:       ", font=("Arial", 12))
        self.key_label.pack(side=tk.LEFT, padx=5, pady=5)

        self.key_entry = ttk.Entry(frame2, width=20, font=("Arial", 12))
        self.key_entry.pack(fill=tk.X, padx=15, expand=True)

        frame3 = ttk.Frame(self.root)
        frame3.pack(fill=tk.X, padx=5, pady=5)

        self.cipher_decipher_label = ttk.Label(
            frame3, text="Mode:     ", font=("Arial", 12)
        )
        self.cipher_decipher_label.pack(side=tk.LEFT)

        self.cipher_decipher_dropdown = ttk.OptionMenu(
            frame3,
            self._cipher_decipher_var,
            CipherMode.CIPHER.name,
            *[CipherMode.DECIPHER.name]
        )
        self.cipher_decipher_dropdown.pack(fill=tk.X, padx=15, expand=False)

        frame4 = ttk.Frame(self.root)
        frame4.pack(fill=tk.X, padx=5, pady=5, expand=True)

        self.text_area = tk.Text(frame4, width=20, height=5)
        self.text_area.pack(fill=tk.BOTH, padx=15, expand=True)

        frame5 = ttk.Frame(self.root)
        frame5.pack(fill=tk.X, padx=15, pady=25)

        self.cipher_button = ttk.Button(frame5, text="Run", command=self.run_algorithm)
        self.cipher_button.pack(side=tk.RIGHT)

    def run(self) -> None:
        """
        Starts the main loop of the GUI.
        """
        self.root.mainloop()

    @property
    def cipher_decipher_var(self) -> CipherMode:
        """
        Cipher mode getter.
        """
        if self._cipher_decipher_var.get() == CipherMode.CIPHER.name:
            return CipherMode.CIPHER
        return CipherMode.DECIPHER

    @cipher_decipher_var.setter
    def cipher_decipher_var(self, value: CipherMode) -> None:
        """
        Cipher mode setter.
        """
        self._cipher_decipher_var.set(value.name)

    def run_algorithm(self) -> None:
        """
        Runs the algorithm.
        Validates the input and shows a popup if the input is invalid.
        """

        if self.message_entry.get() == "":
            self.show_popup("Message is empty!")
            return
        try:
            int(self.key_entry.get())
        except ValueError:
            self.show_popup("Key is not a number!")
            return

        if self.cipher_decipher_var == CipherMode.CIPHER:
            self.cipher()
        else:
            self.decipher()

    def show_popup(self, message: str) -> None:
        """
        Shows a popup with a message.
        """
        popup = StyledWindow()
        popup.title("Error")
        popup.geometry("300x100")
        label = ttk.Label(popup, text=message, font=("Arial", 12))
        label.pack(fill=tk.X, padx=15, expand=True)
        button = ttk.Button(popup, text="OK", command=popup.destroy)
        button.pack(fill=tk.X, padx=15, expand=True)
        popup.mainloop()

    def cipher(self) -> None:
        """
        Ciphers the message and displays it in the text area.
        """
        self.message = self.message_entry.get()
        self.key = int(self.key_entry.get())
        self.cipher_message = caesar_encrypt(self.message, self.key)
        self.text_area.delete(1.0, tk.END)
        self.text_area.insert(tk.END, self.cipher_message)

    def decipher(self) -> None:
        """
        Deciphers the message and displays it in the text area.
        """
        self.message = self.message_entry.get()
        self.key = int(self.key_entry.get())
        self.decipher_message = caesar_decrypt(self.message, self.key)
        self.text_area.delete(1.0, tk.END)
        self.text_area.insert(tk.END, self.decipher_message)
