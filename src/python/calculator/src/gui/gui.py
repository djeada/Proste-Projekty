import tkinter as tk
from tkinter import ttk

from src.python.calculator.src.gui.sytled_window import StyledWindow
from src.python.calculator.src.logic.calculator import Calculator


class Gui:
    """
    Application window and its widgets.
    """

    def __init__(self):
        self.root = StyledWindow()
        self.root.title("Calculator")
        self.root.geometry("440x300")
        self.root.resizable(False, False)

        self.button_equals = None
        self.button_clear = None
        self.button_zero = None
        self.button_decimal = None
        self.button_add = None
        self.button_three = None
        self.button_two = None
        self.button_one = None
        self.button_subtract = None
        self.button_six = None
        self.button_five = None
        self.button_four = None
        self.button_multiply = None
        self.button_nine = None
        self.button_eight = None
        self.button_seven = None
        self.button_divide = None
        self.button_sqrt = None
        self.button_square = None
        self.output_label = None
        self.button_inverse = None
        self.output_entry = None
        self.input_entry = None
        self.input_label = None
        self.create_widgets()

    def create_widgets(self) -> None:
        """
        Creates all the widgets.
        """

        frame1 = ttk.Frame(self.root)
        frame1.pack(fill=tk.X, padx=5, pady=5)

        self.input_label = ttk.Label(frame1, text="Input:")
        self.input_label.grid(row=0, column=0, sticky="nesw")

        self.input_entry = ttk.Entry(frame1)
        self.input_entry.grid(row=0, column=1, sticky="nesw")

        self.output_label = ttk.Label(frame1, text="Output:")
        self.output_label.grid(row=1, column=0, sticky="nesw")

        self.output_entry = ttk.Entry(frame1)
        self.output_entry.grid(row=1, column=1, sticky="nesw")

        # 1/x | x^2 | sqrt(x) | /
        # 7   | 8    | 9      | *
        # 4   | 5    | 6      | -
        # 1   | 2    | 3      | +
        # .   | 0    | C      | =

        frame2 = ttk.Frame(self.root, relief="sunken")
        frame2.pack(fill=tk.X, padx=10, pady=5)

        self.button_inverse = ttk.Button(frame2, text="   1/x   ")
        self.button_inverse.grid(row=2, column=0, sticky="nesw")
        self.button_inverse.configure(command=self.invert)

        self.button_square = ttk.Button(frame2, text="   x^2   ")
        self.button_square.grid(row=2, column=1, sticky="nesw")
        self.button_square.configure(command=self.square)

        self.button_sqrt = ttk.Button(frame2, text="sqrt(x)")
        self.button_sqrt.grid(row=2, column=2, sticky="nesw")
        self.button_sqrt.configure(command=self.square_root)

        self.button_divide = ttk.Button(frame2, text="    /    ")
        self.button_divide.grid(row=2, column=3, sticky="nesw")
        self.button_divide.configure(command=lambda: self.update_input_entry("/"))

        self.button_seven = ttk.Button(frame2, text="7")
        self.button_seven.grid(row=3, column=0, sticky="nesw")
        self.button_seven.configure(command=lambda: self.update_input_entry("7"))

        self.button_eight = ttk.Button(frame2, text="8")
        self.button_eight.grid(row=3, column=1, sticky="nesw")
        self.button_eight.configure(command=lambda: self.update_input_entry("8"))

        self.button_nine = ttk.Button(frame2, text="9")
        self.button_nine.grid(row=3, column=2, sticky="nesw")
        self.button_nine.configure(command=lambda: self.update_input_entry("9"))

        self.button_multiply = ttk.Button(frame2, text="*")
        self.button_multiply.grid(row=3, column=3, sticky="nesw")
        self.button_multiply.configure(command=lambda: self.update_input_entry("*"))

        self.button_four = ttk.Button(frame2, text="4")
        self.button_four.grid(row=4, column=0, sticky="nesw")
        self.button_four.configure(command=lambda: self.update_input_entry("4"))

        self.button_five = ttk.Button(frame2, text="5")
        self.button_five.grid(row=4, column=1, sticky="nesw")
        self.button_five.configure(command=lambda: self.update_input_entry("5"))

        self.button_six = ttk.Button(frame2, text="6")
        self.button_six.grid(row=4, column=2, sticky="nesw")
        self.button_six.configure(command=lambda: self.update_input_entry("6"))

        self.button_subtract = ttk.Button(frame2, text="-")
        self.button_subtract.grid(row=4, column=3, sticky="nesw")
        self.button_subtract.configure(command=lambda: self.update_input_entry("-"))

        self.button_one = ttk.Button(frame2, text="1")
        self.button_one.grid(row=5, column=0, sticky="nesw")
        self.button_one.configure(command=lambda: self.update_input_entry("1"))

        self.button_two = ttk.Button(frame2, text="2")
        self.button_two.grid(row=5, column=1, sticky="nesw")
        self.button_two.configure(command=lambda: self.update_input_entry("2"))

        self.button_three = ttk.Button(frame2, text="3")
        self.button_three.grid(row=5, column=2, sticky="nesw")
        self.button_three.configure(command=lambda: self.update_input_entry("3"))

        self.button_add = ttk.Button(frame2, text="+")
        self.button_add.grid(row=5, column=3, sticky="nesw")
        self.button_add.configure(command=lambda: self.update_input_entry("+"))

        self.button_decimal = ttk.Button(frame2, text=".")
        self.button_decimal.grid(row=6, column=0, sticky="nesw")
        self.button_decimal.configure(command=lambda: self.update_input_entry("."))

        self.button_zero = ttk.Button(frame2, text="0")
        self.button_zero.grid(row=6, column=1, sticky="nesw")
        self.button_zero.configure(command=lambda: self.update_input_entry("0"))

        self.button_clear = ttk.Button(frame2, text="C")
        self.button_clear.grid(row=6, column=2, sticky="nesw")
        self.button_clear.configure(command=self.clear)

        self.button_equals = ttk.Button(frame2, text="=")
        self.button_equals.grid(row=6, column=3, sticky="nesw")
        self.button_equals.configure(command=self.calculate)

    def run(self) -> None:
        """
        Starts the main loop of the GUI.
        """
        self.root.mainloop()

    def clear(self) -> None:
        """
        Clear the input entry and output entry.
        """
        self.input_entry.delete(0, tk.END)
        self.output_entry.delete(0, tk.END)

    def update_input_entry(self, value: str) -> None:
        """
        Update the input entry with the given value.
        """

        current_value = self.input_entry.get()

        if not Calculator(current_value + value + "1").validate_calculation_string():
            return

        if value in ["+", "-", "*", "/"]:
            value = " " + value + " "

        self.input_entry.delete(0, tk.END)
        self.input_entry.insert(0, current_value + value)

    def invert(self) -> None:
        """
        Inverts the value in the input entry.
        """
        current_value = self.input_entry.get()

        if current_value == "":
            return

        calculator = Calculator(current_value)
        calculator.invert()
        calculation_result = calculator.eval_calculation_string()
        self.output_entry.delete(0, "end")
        self.output_entry.insert(0, calculation_result)

    def square(self) -> None:
        """
        Squares the value in the input entry.
        """
        current_value = self.input_entry.get()

        if current_value == "":
            return

        calculator = Calculator(current_value)
        calculator.square()
        calculation_result = calculator.eval_calculation_string()
        self.output_entry.delete(0, "end")
        self.output_entry.insert(0, calculation_result)

    def square_root(self) -> None:
        """
        Squares the value in the input entry.
        """
        current_value = self.input_entry.get()

        if current_value == "":
            return

        calculator = Calculator(current_value)
        calculator.square_root()
        calculation_result = calculator.eval_calculation_string()
        self.output_entry.delete(0, "end")
        self.output_entry.insert(0, calculation_result)

    def calculate(self) -> None:
        """
        Calculate button action
        """
        calculation_string = self.input_entry.get()
        calculator = Calculator(calculation_string)
        if not calculator.validate_calculation_string():
            calculation_result = "Can't parse the expression"
        else:
            calculation_result = calculator.eval_calculation_string()
        self.output_entry.delete(0, "end")
        self.output_entry.insert(0, calculation_result)
