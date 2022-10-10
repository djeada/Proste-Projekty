from typing import List

from src.python.yahtzee.src.logic.player import PlayerType, Player
from src.python.yahtzee.src.logic.table import ScoreType


class Game:
    """
    Model of the YAHTZE game.
    """

    def __init__(self):
        self.player_a = Player(PlayerType.A)
        self.player_b = Player(PlayerType.B)
        self._current_player_type: PlayerType = PlayerType.A

    @property
    def current_player(self) -> Player:
        """
        Gets the current player.

        :return: current player
        """
        return (
            self.player_a
            if self._current_player_type == PlayerType.A
            else self.player_b
        )

    @current_player.setter
    def current_player(self, player: Player) -> None:
        """
        Sets the current player.

        :param player: player to set as current
        """
        self._current_player_type = player.player_type

    def roll_dice(self) -> None:
        """
        Rolls the dice.
        """
        self.current_player.roll_dice()

        if self.current_player.numbers_of_throws_left == 0:
            self.current_player.reset()
            self.switch_player()

    def switch_player(self) -> None:
        """
        Switches the current player to the other one.
        """
        if self.current_player == self.player_a:
            self.current_player = self.player_b
        else:
            self.current_player = self.player_a

    def put_away_dice(self, dice_list: List[int]) -> None:
        """
        Puts away a list of dice.

        :param dice_list: list of dice to put away
        """
        self.current_player.dice_list_put_away = dice_list

    def update_current_player_table(self, score_type: ScoreType) -> None:
        """
        Updates the current player's table with the score_type.

        :param score_type: score_type to update the table with
        """
        self.current_player.update_table(score_type)
        self.current_player.reset()
        self.switch_player()
