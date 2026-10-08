"""Tkinter interface: a button grid and keyboard input. All arithmetic is done by calculator.evaluate."""
import tkinter as tk
import tkinter.font as tkfont

from calculator import CalculatorError, evaluate

# (label on the button, key it stands for)
BUTTONS = [
    [('(', '('), (')', ')'), ('Del', 'Backspace'), ('C', 'C')],
    [('7', '7'), ('8', '8'), ('9', '9'), ('÷', '/')],
    [('4', '4'), ('5', '5'), ('6', '6'), ('×', '*')],
    [('1', '1'), ('2', '2'), ('3', '3'), ('-', '-')],
    [('0', '0'), ('.', '.'), ('=', '='), ('+', '+')],
]
KEYSYM_KEYS = {'Return': '=', 'KP_Enter': '=', 'Escape': 'C', 'BackSpace': 'Backspace'}
ALLOWED_KEYS = set('0123456789.+-*/()=') | {'C', 'Backspace'}


class CalculatorWindow:
    def __init__(self, root):
        self.expression = ''
        self.history = ''  # the expression that produced the value on the display
        self.shows_result = False

        root.title('Calculator')
        big = tkfont.nametofont('TkDefaultFont').copy()
        big.configure(size=20, weight='bold')
        small = tkfont.nametofont('TkDefaultFont').copy()
        small.configure(size=11)

        self.history_label = tk.Label(root, anchor='e', font=small, fg='gray', padx=10)
        self.history_label.grid(row=0, column=0, columnspan=4, sticky='ew')
        self.display = tk.Label(root, anchor='e', font=big, padx=10, pady=6)
        self.display.grid(row=1, column=0, columnspan=4, sticky='ew')
        self.status = tk.Label(root, anchor='e', fg='red', padx=10)
        self.status.grid(row=2, column=0, columnspan=4, sticky='ew')

        for row_index, row in enumerate(BUTTONS, start=3):
            for column, (label, key) in enumerate(row):
                button = tk.Button(root, text=label, font=big, width=3, height=1,
                                   command=lambda k=key: self.press(k))
                button.grid(row=row_index, column=column, sticky='nsew', padx=2, pady=2)
        for column in range(4):
            root.columnconfigure(column, weight=1)

        root.bind('<Key>', self.on_key)
        self.refresh()

    def on_key(self, event):
        key = KEYSYM_KEYS.get(event.keysym, event.char)
        if key in ALLOWED_KEYS:
            self.press(key)

    def press(self, key):
        self.status.config(text='')
        if key == '=':
            self.calculate()
            return
        if key == 'C':
            self.expression = ''
        elif key == 'Backspace':
            self.expression = self.expression[:-1]
        else:
            if self.shows_result and key not in '+-*/':
                self.expression = ''  # a new number starts a new calculation
            self.expression += key
        self.shows_result = False
        self.refresh()

    def calculate(self):
        try:
            value = evaluate(self.expression)
        except CalculatorError as error:
            self.status.config(text=str(error))
        else:
            self.history = self.expression
            self.expression = f'{value:.10g}'
            self.shows_result = True
        self.refresh()

    def refresh(self):
        self.history_label.config(text=self.history)
        self.display.config(text=self.expression or '0')


def main():
    root = tk.Tk()
    CalculatorWindow(root)
    root.mainloop()


if __name__ == '__main__':
    main()
