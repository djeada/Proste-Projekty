import random


class Dice:
    def __init__(self, value: int = -1):
        self.value = value

    def roll(self) -> "Dice":
        self.value = random.randint(1, 6)
        return self

    def invalidate(self) -> "Dice":
        self.value = -1
        return self

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return f"{self.value}"

