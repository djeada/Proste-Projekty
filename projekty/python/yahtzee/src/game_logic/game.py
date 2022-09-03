from typing import List

from src.game_logic.player import PlayerType, Player
from src.game_logic.table import ScoreType


class Game:
    def __init__(self):
        self.player_a = Player(PlayerType.A)
        self.player_b = Player(PlayerType.B)
        self._current_player_type: PlayerType = PlayerType.A

    @property
    def current_player(self) -> Player:
        return self.player_a if self._current_player_type == PlayerType.A else self.player_b
    
    @current_player.setter
    def current_player(self, player: Player) -> None:
        self._current_player_type = player.player_type

    def roll_dice(self) -> None:
        self.current_player.roll_dice()

        if self.current_player.numbers_of_throws_left == 0:
            self.current_player.reset()
            self.switch_player()

    def switch_player(self) -> None:
        if self.current_player == self.player_a:
            self.current_player = self.player_b
        else:
            self.current_player = self.player_a

    def put_away_dice(self, dice_list: List[int]) -> None:
        self.current_player.dice_list_put_away = dice_list

    def update_current_player_table(self, score_type: ScoreType) -> None:
        self.current_player.update_table(score_type)
        self.current_player.reset()
        self.switch_player()