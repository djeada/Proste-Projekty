import random


class Dice:
    """
    Represents a dice.
    """

    def __init__(self, value: int = -1):
        self.value = value

    def roll(self) -> "Dice":
        """
        Chooses a random value between 1 and 6.

        :return: self
        """
        self.value = random.randint(1, 6)
        return self

    def invalidate(self) -> "Dice":
        """
        Invalidates the dice.

        :return: self
        """
        self.value = -1
        return self

    def __repr__(self) -> str:
        """
        String representation of the dice.

        :return: string representation of the dice
        """
        return self.__str__()

    def __str__(self) -> str:
        """
        String representation of the dice.

        :return: string representation of the dice
        """
        return f"{self.value}"
