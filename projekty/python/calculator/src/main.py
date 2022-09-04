import tkinter as tk


class Calculator:

    def __init__(self, calculation_string: str):
        self.calculation_string = calculation_string

    def validate_calculation_string(self):
        '''
        Validate calculation string
        '''
        try:
            eval( self.calculation_string)
            return True
        except:
            return False

    def invert(self):
        '''
        Invert calculation string
        '''
        self.        calculation_string = f'1/({ self.calculation_string})'

    def square(self):
        '''
        Square calculation string
        '''
        self.calculation_string = f'({self.calculation_string})**2'

    def square_root(self):
        '''
        Square root calculation string
        '''
        self.calculation_string = f'({self.calculation_string})**0.5'

    def eval_calculation_string(self):
        '''
        Evaluate calculation string
        '''
        return eval(self.calculation_string)


class Gui:

    def __init__(self, root):
        self.root = root
        self.root.title('Calculator')
        self.root.geometry('200x200')
        self.root.resizable(False, False)
        self.root.configure(background='#f2f2f2')

        self.create_widgets()

    def create_widgets(self) -> None:
        '''
        Create widgets for GUI
        '''

        frame1 = tk.Frame(self.root)
        frame1.pack(fill=tk.X, padx=5, pady=5)

        self.input_label = tk.Label(frame1, text='Input:')
        self.input_label.configure(background='#f2f2f2')
        self.input_label.grid(row=0, column=0, sticky='nesw')

        self.input_entry = tk.Entry(frame1)
        self.input_entry.grid(row=0, column=1, sticky='nesw')

        self.output_label = tk.Label(frame1, text='Output:')
        self.output_label.configure(background='#f2f2f2')
        self.output_label.grid(row=1, column=0, sticky='nesw')

        self.output_entry = tk.Entry(frame1)
        self.output_entry.grid(row=1, column=1, sticky='nesw')

        # 1/x | x^2 | sqrt(x) | /
        # 7   | 8    | 9      | *
        # 4   | 5    | 6      | -
        # 1   | 2    | 3      | +
        # .   | 0    | C      | =

        frame2 = tk.Frame(self.root, relief='sunken')
        frame2.pack(fill=tk.X, padx=10, pady=5)

        self.button_inverse = tk.Button(frame2, text='   1/x   ')
        self.button_inverse.grid(row=2, column=0, sticky='nesw')
        self.button_inverse.configure(command=self.invert)

        self.button_square = tk.Button(frame2, text='   x^2   ')
        self.button_square.grid(row=2, column=1, sticky='nesw')
        self.button_square.configure(command=self.square)

        self.button_sqrt = tk.Button(frame2, text='sqrt(x)')
        self.button_sqrt.grid(row=2, column=2, sticky='nesw')
        self.button_sqrt.configure(command=self.square_root)

        self.button_divide = tk.Button(frame2, text='    /    ')
        self.button_divide.grid(row=2, column=3, sticky='nesw')
        self.button_divide.configure(command=lambda: self.update_input_entry('/'))

        self.button_seven = tk.Button(frame2, text='7')
        self.button_seven.grid(row=3, column=0, sticky='nesw')
        self.button_seven.configure(command=lambda: self.update_input_entry('7'))

        self.button_eight = tk.Button(frame2, text='8')
        self.button_eight.grid(row=3, column=1, sticky='nesw')
        self.button_eight.configure(command=lambda: self.update_input_entry('8'))

        self.button_nine = tk.Button(frame2, text='9')
        self.button_nine.grid(row=3, column=2, sticky='nesw')
        self.button_nine.configure(command=lambda: self.update_input_entry('9'))

        self.button_multiply = tk.Button(frame2, text='*')
        self.button_multiply.grid(row=3, column=3, sticky='nesw')
        self.button_multiply.configure(command=lambda: self.update_input_entry('*'))

        self.button_four = tk.Button(frame2, text='4')
        self.button_four.grid(row=4, column=0, sticky='nesw')
        self.button_four.configure(command=lambda: self.update_input_entry('4'))

        self.button_five = tk.Button(frame2, text='5')
        self.button_five.grid(row=4, column=1, sticky='nesw')
        self.button_five.configure(command=lambda: self.update_input_entry('5'))

        self.button_six = tk.Button(frame2, text='6')
        self.button_six.grid(row=4, column=2, sticky='nesw')
        self.button_six.configure(command=lambda: self.update_input_entry('6'))

        self.button_subtract = tk.Button(frame2, text='-')
        self.button_subtract.grid(row=4, column=3, sticky='nesw')
        self.button_subtract.configure(command=lambda: self.update_input_entry('-'))

        self.button_one = tk.Button(frame2, text='1')
        self.button_one.grid(row=5, column=0, sticky='nesw')
        self.button_one.configure(command=lambda: self.update_input_entry('1'))

        self.button_two = tk.Button(frame2, text='2')
        self.button_two.grid(row=5, column=1, sticky='nesw')
        self.button_two.configure(command=lambda: self.update_input_entry('2'))

        self.button_three = tk.Button(frame2, text='3')
        self.button_three.grid(row=5, column=2, sticky='nesw')
        self.button_three.configure(command=lambda: self.update_input_entry('3'))

        self.button_add = tk.Button(frame2, text='+')
        self.button_add.grid(row=5, column=3, sticky='nesw')
        self.button_add.configure(command=lambda: self.update_input_entry('+'))

        self.button_decimal = tk.Button(frame2, text='.')
        self.button_decimal.grid(row=6, column=0, sticky='nesw')
        self.button_decimal.configure(command=lambda: self.update_input_entry('.'))

        self.button_zero = tk.Button(frame2, text='0')
        self.button_zero.grid(row=6, column=1, sticky='nesw')
        self.button_zero.configure(command=lambda: self.update_input_entry('0'))

        self.button_clear = tk.Button(frame2, text='C')
        self.button_clear.grid(row=6, column=2, sticky='nesw')
        self.button_clear.configure(command=self.clear)

        self.button_equals = tk.Button(frame2, text='=')
        self.button_equals.grid(row=6, column=3, sticky='nesw')
        self.button_equals.configure(command=self.calculate)

    def clear(self):
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

        if not Calculator(current_value + value + '1').validate_calculation_string():
            return

        if value in ['+', '-', '*', '/']:
            value = ' ' + value + ' '

        self.input_entry.delete(0, tk.END)
        self.input_entry.insert(0, current_value + value)

    def invert(self) -> None:
        """
        Inverts the value in the input entry.
        """
        current_value = self.input_entry.get()

        if current_value == '':
            return

        calculator = Calculator(current_value)
        calculator.invert()
        calculation_result = calculator.eval_calculation_string()
        self.output_entry.delete(0, 'end')
        self.output_entry.insert(0, calculation_result)

    def square(self) -> None:
        """
        Squares the value in the input entry.
        """
        current_value = self.input_entry.get()

        if current_value == '':
            return

        calculator = Calculator(current_value)
        calculator.square()
        calculation_result = calculator.eval_calculation_string()
        self.output_entry.delete(0, 'end')
        self.output_entry.insert(0, calculation_result)

    def square_root(self) -> None:
        """
        Squares the value in the input entry.
        """
        current_value = self.input_entry.get()

        if current_value == '':
            return

        calculator = Calculator(current_value)
        calculator.square_root()
        calculation_result = calculator.eval_calculation_string()
        self.output_entry.delete(0, 'end')
        self.output_entry.insert(0, calculation_result)

    def calculate(self) -> None:
        '''
        Calculate button action
        '''
        calculation_string = self.input_entry.get()
        calculator = Calculator(calculation_string)
        if not calculator.validate_calculation_string():
            calculation_result = "Can't parse the expression"
        else:
            calculation_result = calculator.eval_calculation_string()
        self.output_entry.delete(0, 'end')
        self.output_entry.insert(0, calculation_result)


def main() -> None:
    root = tk.Tk()
    gui = Gui(root)
    root.mainloop()


if __name__ == '__main__':
    main()