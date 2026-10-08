"""Tkinter window for the Caesar cipher."""

import tkinter as tk
from tkinter import messagebox, ttk
from typing import Optional

from caesar_cipher import crack, decrypt, encrypt


class CipherWindow:
    """The main window: input text, key and buttons, then the result."""

    def __init__(self, root: tk.Tk):
        self.root = root
        root.title("Caesar Cipher")

        self.text_input = tk.Text(root, height=5, width=50, wrap=tk.WORD)
        self.text_input.pack(fill=tk.BOTH, padx=10, pady=(10, 5))

        controls = ttk.Frame(root)
        controls.pack(fill=tk.X, padx=10, pady=5)
        ttk.Label(controls, text="Key:").pack(side=tk.LEFT)
        self.key_entry = ttk.Entry(controls, width=6)
        self.key_entry.insert(0, "3")
        self.key_entry.pack(side=tk.LEFT, padx=(5, 15))
        ttk.Button(controls, text="Encrypt", command=self.on_encrypt).pack(side=tk.LEFT, padx=2)
        ttk.Button(controls, text="Decrypt", command=self.on_decrypt).pack(side=tk.LEFT, padx=2)
        ttk.Button(controls, text="Crack", command=self.on_crack).pack(side=tk.LEFT, padx=2)

        self.status = ttk.Label(root, text="")
        self.status.pack(fill=tk.X, padx=10)

        self.result = tk.Text(root, height=5, width=50, wrap=tk.WORD, state=tk.DISABLED)
        self.result.pack(fill=tk.BOTH, padx=10, pady=(5, 10))

    def read_text(self) -> str:
        return self.text_input.get("1.0", "end-1c")

    def read_key(self) -> Optional[int]:
        try:
            return int(self.key_entry.get())
        except ValueError:
            messagebox.showerror("Caesar Cipher", "The key must be a whole number.")
            return None

    def show_result(self, text: str) -> None:
        self.result.config(state=tk.NORMAL)
        self.result.delete("1.0", tk.END)
        self.result.insert("1.0", text)
        self.result.config(state=tk.DISABLED)

    def on_encrypt(self) -> None:
        key = self.read_key()
        if key is not None:
            self.status.config(text="")
            self.show_result(encrypt(self.read_text(), key))

    def on_decrypt(self) -> None:
        key = self.read_key()
        if key is not None:
            self.status.config(text="")
            self.show_result(decrypt(self.read_text(), key))

    def on_crack(self) -> None:
        key, plain_text = crack(self.read_text())
        self.key_entry.delete(0, tk.END)
        self.key_entry.insert(0, str(key))
        self.status.config(text=f"Most likely key: {key} (the result is the decrypted text)")
        self.show_result(plain_text)


def main():
    root = tk.Tk()
    CipherWindow(root)
    root.mainloop()


if __name__ == "__main__":
    main()
