from dataclasses import dataclass


@dataclass
class Position:
    """
    Representation of a position on the board.
    """

    x: int
    y: int

    def __add__(self, other: "Position") -> "Position":
        """
        Overloads the + operator.

        return: Position that is the sum of the two positions.
        """
        return Position(self.x + other.x, self.y + other.y)

    def __sub__(self, other: "Position") -> "Position":
        """
        Overloads the - operator.

        return: Position that is the difference of the two positions.
        """
        return Position(self.x - other.x, self.y - other.y)

    def __eq__(self, other: "Position") -> bool:
        """
        Overloads the == operator.

        return: True if the positions are equal and False otherwise.
        """
        return self.x == other.x and self.y == other.y

    def __hash__(self) -> int:
        """
        Maps the position to an integer.

        return: Integer representation of the position.
        """
        return hash((self.x, self.y))

    def __repr__(self) -> str:
        """
        Turns the position into a string.

        return: String representation of the position.
        """
        return f"({self.x}, {self.y})"

    def __str__(self) -> str:
        """
        Turns the position into a string.

        return: String representation of the position.
        """
        return f"({self.x}, {self.y})"
