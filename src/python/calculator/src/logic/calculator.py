class Calculator:
    """
    Defines various methods for the calculator.
    Parses and evaluates the calculation string.
    """

    def __init__(self, calculation_string: str):
        self.calculation_string = calculation_string

    def validate_calculation_string(self) -> bool:
        """
        Validates the calculation string.
        Return: True if the calculation string is valid, False otherwise.
        """
        try:
            eval(self.calculation_string)
            return True
        except:
            return False

    def invert(self) -> None:
        """
        Inverts the number represented by the calculation string.
        """
        self.calculation_string = f"1/({ self.calculation_string})"

    def square(self):
        """
        Squares the number represented by the calculation string.
        """
        self.calculation_string = f"({self.calculation_string})**2"

    def square_root(self):
        """
        Calculates the square root of the number represented by the calculation string.
        """
        self.calculation_string = f"({self.calculation_string})**0.5"

    def eval_calculation_string(self):
        """
        Evaluate the calculation string.
        """
        return eval(self.calculation_string)
