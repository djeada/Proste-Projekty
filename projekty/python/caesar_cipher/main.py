import enum
import tkinter as tk
from enum import Enum
#from cyberpunk_theme import Cyberpunk

class CeasarCipher:
    def __init__(self, message: str, key: int):
        self.message = message
        self.key = key

    def cipher(self):
        cipher_message = ''
        for letter in self.message:
            if letter.isalpha():
                if letter.isupper():
                    cipher_message += chr((ord(letter) + self.key - ord('A')) % 26 +  ord('A'))
                else:
                    cipher_message += chr((ord(letter) + self.key -  ord('a')) % 26 +  ord('a'))
            else:
                cipher_message += letter
        return cipher_message

    def decipher(self):
        decipher_message = ''
        for letter in self.message:
            if letter.isalpha():
                if letter.isupper():
                    decipher_message += chr((ord(letter) - self.key - ord('A')) % 26 +  ord('A'))
                else:
                    decipher_message += chr((ord(letter) - self.key - ord('a')) % 26 +  ord('a'))
            else:
                decipher_message += letter
        return decipher_message


class CipherMode(Enum):
    CIPHER = enum.auto()
    DECIPHER = enum.auto()


class Gui:
    def __init__(self, root):
        self._cipher_decipher_var = tk.StringVar()
        self._cipher_decipher_var.set(CipherMode.CIPHER.name)

        self.root = root
        self.root.title('Caesar Cipher')
        self.root.geometry('300x300')
        self.root.configure(background='#f2f2f2')

        frame1 = tk.Frame(self.root)
        frame1.pack(fill=tk.X, padx=5, pady=5)

        self.message_label = tk.Label(frame1, text='Message:', bg='#f2f2f2', font=('Arial', 12))
        self.message_label.pack(side=tk.LEFT)

        self.message_entry = tk.Entry(frame1, width=20, font=('Arial', 12))
        self.message_entry.pack(fill=tk.X, padx=15, expand=True)

        frame2 = tk.Frame(self.root)
        frame2.pack(fill=tk.X, padx=5, pady=5)

        self.key_label = tk.Label(frame2, text='Key:       ', bg='#f2f2f2', font=('Arial', 12))
        self.key_label.pack(side=tk.LEFT, padx=5, pady=5)

        self.key_entry = tk.Entry(frame2, width=20, font=('Arial', 12))
        self.key_entry.pack(fill=tk.X, padx=15, expand=True)

        frame3 = tk.Frame(self.root)
        frame3.pack(fill=tk.X, padx=5, pady=5)

        self.cipher_decipher_label = tk.Label(frame3, text='Mode:     ', bg='#f2f2f2', font=('Arial', 12))
        self.cipher_decipher_label.pack(side=tk.LEFT)

        self.cipher_decipher_dropdown = tk.OptionMenu(frame3, self._cipher_decipher_var,CipherMode.CIPHER.name, *[CipherMode.DECIPHER.name,])
        self.cipher_decipher_dropdown.pack(fill=tk.X, padx=15, expand=False)

        frame4 = tk.Frame(self.root)
        frame4.pack(fill=tk.X, padx=5, pady=5, expand=True)

        self.text_area = tk.Text(frame4, width=20, height=5, font=('Arial', 12))
        self.text_area.pack(fill=tk.BOTH, padx=15, expand=True)

        frame5 = tk.Frame(self.root)
        frame5.pack(fill=tk.X, padx=15, pady=25)

        self.cipher_button = tk.Button(frame5, text='Run', command=self.run_algorithm, font=('Arial', 12))
        self.cipher_button.pack(side=tk.RIGHT)

    @property
    def cipher_decipher_var(self) -> CipherMode:
        if self._cipher_decipher_var.get() == CipherMode.CIPHER.name:
            return CipherMode.CIPHER
        return CipherMode.DECIPHER

    @cipher_decipher_var.setter
    def cipher_decipher_var(self, value: CipherMode):
        self._cipher_decipher_var.set(value.name)

    def run_algorithm(self):
        # validate input and show popup if input is invalid
        if self.message_entry.get() == '':
            self.show_popup('Message is empty')
            return
        try:
            int(self.key_entry.get())
        except ValueError:
            self.show_popup('Key is not a number')
            return

        if self.cipher_decipher_var == CipherMode.CIPHER:
            self.cipher()
        else:
            self.decipher()

    def show_popup(self, message: str):
        popup = tk.Tk()
        popup.title('Error')
        popup.geometry('300x100')
        popup.configure(background='#f2f2f2')
        label = tk.Label(popup, text=message, bg='#f2f2f2', font=('Arial', 12))
        label.pack(fill=tk.X, padx=15, expand=True)
        button = tk.Button(popup, text='OK', command=popup.destroy, font=('Arial', 12))
        button.pack(fill=tk.X, padx=15, expand=True)
        popup.mainloop()

    def cipher(self):
        self.message = self.message_entry.get()
        self.key = int(self.key_entry.get())
        self.cipher_message = CeasarCipher(self.message, self.key).cipher()
        self.text_area.delete(1.0, tk.END)
        self.text_area.insert(tk.END, self.cipher_message)

    def decipher(self):
        self.message = self.message_entry.get()
        self.key = int(self.key_entry.get())
        self.decipher_message = CeasarCipher(self.message, self.key).decipher()
        self.text_area.delete(1.0, tk.END)
        self.text_area.insert(tk.END, self.decipher_message)


def main():
    root = tk.Tk()
    # apply the Cyberpunk theme to the GUI
    #cyberpunk_theme = Cyberpunk()
    #cyberpunk_theme.target(root)
    gui = Gui(root)
    root.mainloop()


if __name__ == '__main__':
    main()