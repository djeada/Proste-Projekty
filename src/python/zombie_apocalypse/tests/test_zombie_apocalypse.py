import math

import pytest

from zombie_apocalypse import (
    KILL_SCORE,
    MAX_HEALTH,
    PLAYER_SPEED,
    WAVE_BREAK,
    WORLD_HEIGHT,
    WORLD_WIDTH,
    ZOMBIE_DAMAGE,
    Bullet,
    Input,
    new_game,
    next_random,
    update,
    wave_size,
    zombie_speed,
)


def empty_field(seed=1):
    """A game with the player in the middle and no zombies, pickups or spawning."""
    game = new_game(seed)
    game.to_spawn = 0
    game.zombies = []
    game.pickups = []
    game.pickup_timer = 1000.0
    return game


def dist(a, b):
    return math.hypot(a[0] - b[0], a[1] - b[1])


def test_new_game_starts_with_wave_one():
    game = new_game(42)
    assert game.health == MAX_HEALTH
    assert game.wave == 1
    assert game.to_spawn == wave_size(1)
    assert game.score == 0
    assert not game.game_over


def test_player_moves_and_stays_in_bounds():
    game = empty_field()
    update(game, Input(move_x=-1.0), 1.0)
    assert game.player[0] == pytest.approx(WORLD_WIDTH / 2 - PLAYER_SPEED)
    for _ in range(100):
        update(game, Input(move_x=-1.0), 0.5)
    assert game.player[0] == pytest.approx(0.5)


def test_diagonal_move_is_not_faster():
    game = empty_field()
    update(game, Input(move_x=1.0, move_y=1.0), 1.0)
    moved = dist(game.player, (WORLD_WIDTH / 2, WORLD_HEIGHT / 2))
    assert moved == pytest.approx(PLAYER_SPEED)


def test_shot_flies_in_aim_direction():
    game = empty_field()
    update(game, Input(aim_x=0.0, aim_y=-5.0, fire=True), 0.01)
    assert len(game.bullets) == 1
    vx, vy = game.bullets[0].vel
    assert vx == pytest.approx(0.0)
    assert vy < 0


def test_zero_aim_uses_last_move_direction():
    game = empty_field()
    update(game, Input(move_y=1.0), 0.01)
    update(game, Input(fire=True), 0.01)
    assert len(game.bullets) == 1
    vx, vy = game.bullets[0].vel
    assert vy > 0
    assert vx == pytest.approx(0.0)


def test_shot_cooldown_limits_fire_rate():
    game = empty_field()
    shoot = Input(aim_x=1.0, fire=True)
    update(game, shoot, 0.01)
    update(game, shoot, 0.01)
    assert len(game.bullets) == 1
    for _ in range(30):
        update(game, shoot, 0.01)
    assert len(game.bullets) >= 2


def test_bullet_leaving_the_world_is_removed():
    game = empty_field()
    game.bullets = [Bullet(pos=(WORLD_WIDTH - 0.1, 5.0), vel=(18.0, 0.0))]
    update(game, Input(), 0.1)
    assert game.bullets == []


def test_zombies_spawn_on_the_edge():
    game = new_game(7)
    game.spawn_timer = 0.0
    update(game, Input(), 0.0)  # dt 0: the zombie does not move after spawning
    assert len(game.zombies) == 1
    x, y = game.zombies[0]
    assert x in (0.0, float(WORLD_WIDTH)) or y in (0.0, float(WORLD_HEIGHT))
    assert game.to_spawn == wave_size(1) - 1


def test_same_seed_gives_same_numbers():
    a = new_game(123)
    b = new_game(123)
    for _ in range(100):
        value = next_random(a)
        assert 0.0 <= value < 1.0
        assert value == next_random(b)


def test_zombie_chases_player():
    game = empty_field()
    game.zombies = [(5.0, 10.0)]
    before = dist(game.zombies[0], game.player)
    update(game, Input(), 0.5)
    after = dist(game.zombies[0], game.player)
    assert after < before
    assert before - after == pytest.approx(zombie_speed(1) * 0.5)


def test_bullet_kills_zombie():
    game = empty_field()
    game.zombies = [(20.0, 10.0)]
    game.bullets = [Bullet(pos=(20.0, 10.0), vel=(0.0, 0.0))]
    update(game, Input(), 0.01)
    assert game.zombies == []
    assert game.bullets == []
    assert game.score == KILL_SCORE


def test_zombie_touch_costs_health():
    game = empty_field()
    game.zombies = [(game.player[0] + 0.5, game.player[1])]
    update(game, Input(), 0.01)
    assert game.zombies == []
    assert game.health == MAX_HEALTH - ZOMBIE_DAMAGE
    assert not game.game_over


def test_zero_health_is_game_over_and_freezes_the_game():
    game = empty_field()
    game.health = ZOMBIE_DAMAGE
    game.zombies = [(game.player[0] + 0.5, game.player[1])]
    update(game, Input(), 0.01)
    assert game.game_over
    assert game.health == 0
    x = game.player[0]
    update(game, Input(move_x=1.0), 1.0)
    assert game.player[0] == x


def test_pickup_heals_up_to_the_maximum():
    game = empty_field()
    game.health = MAX_HEALTH - 5
    game.pickups = [game.player]
    update(game, Input(), 0.01)
    assert game.pickups == []
    assert game.health == MAX_HEALTH


def test_wave_clears_and_the_next_wave_is_harder():
    game = empty_field()
    update(game, Input(), 0.01)
    assert game.wave_delay > 0
    update(game, Input(), WAVE_BREAK + 0.1)
    assert game.wave == 2
    assert game.to_spawn == wave_size(2)
    assert wave_size(2) > wave_size(1)
    assert zombie_speed(2) > zombie_speed(1)
