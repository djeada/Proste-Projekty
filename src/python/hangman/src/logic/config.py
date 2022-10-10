import dataclasses
from pathlib import Path
from typing import Tuple


@dataclasses.dataclass
class Config:
    """
    This class represents the configuration for the game.
    """

    paths: Tuple[Path]
    words: Tuple[str]
