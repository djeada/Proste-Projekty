from typing import List

from src.game_logic.player import PlayerType, Player
from src.game_logic.table import ScoreType


class Game:
    def __init__(self):
        self.player_a = Player(PlayerType.A)
        self.player_b = Player(PlayerType.B)
        self.current_player: Player = self.player_a

    def roll_dices(self) -> None:
        if self.current_player.numbers_of_throws_left == 0:
            self.current_player.reset()
            self.switch_player()

        self.current_player.roll_dices()

    def switch_player(self) -> None:
        if self.current_player == self.player_a:
            self.current_player = self.player_b
        else:
            self.current_player = self.player_a

    def put_away_dices(self, indices: List[int]) -> None:
        self.current_player.dices_put_away = indices

    def update_current_player_table(self, score_type: ScoreType) -> None:
        self.current_player.update_table(score_type)