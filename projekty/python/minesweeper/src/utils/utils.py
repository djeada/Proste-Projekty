from dataclasses import dataclass
from enum import Enum, auto
from tkinter import ttk


class GameState(Enum):
    """
    Enum that holds the states of the game.
    """

    default = auto()
    win = auto()
    loss = auto()


class ButtonState(Enum):
    """
    Enum that holds the states of the buttons.
    """

    default = auto()
    revealed = auto()
    flagged = auto()


@dataclass
class ButtonInfo:
    """
    Holds the information about the widget and the current state of the button.
    """

    widget: ttk.Button
    state: ButtonState
