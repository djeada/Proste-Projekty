import math

import pytest

from shooting_ducks import (
    DUCK_HALF_WIDTH,
    FIELD_WIDTH,
    POINTS_PER_HIT,
    START_LIVES,
    WAVE_BREAK_SECONDS,
    Duck,
    Game,
    Rng,
    ducks_in_wave,
)


def first_duck(game):
    game.update(0.001)
    return game.ducks[0]


def test_new_game_starts_wave_one():
    game = Game(seed=42)
    assert game.wave == 1
    assert game.score == 0
    assert game.lives == START_LIVES
    assert game.ducks == []
    assert game.ducks_to_spawn == ducks_in_wave(1)
    assert not game.game_over


def test_random_source_is_repeatable_and_in_range():
    a, b = Rng(7), Rng(7)
    for _ in range(1000):
        value = a.next()
        assert value == b.next()
        assert 0.0 <= value < 1.0


def test_wave_spawns_all_its_ducks():
    game = Game(seed=3)
    for _ in range(100):
        game.update(0.1)
        if game.ducks_to_spawn == 0:
            break
    assert game.ducks_to_spawn == 0
    assert len(game.ducks) > 0


def test_movement_uses_elapsed_time():
    game = Game(seed=5)
    duck = first_duck(game)
    x, speed = duck.x, duck.speed
    game.update(0.5)
    assert game.ducks[0].x == pytest.approx(x + speed * 0.5)


def test_shooting_hits_and_misses():
    game = Game(seed=5)
    duck = first_duck(game)
    assert game.shoot(duck.x + 50.0, duck.y) is False
    assert game.score == 0
    assert len(game.ducks) == 1

    assert game.shoot(duck.x, duck.y) is True
    assert game.score == POINTS_PER_HIT
    assert game.ducks == []


def test_escaped_duck_costs_a_life():
    game = Game(seed=9)
    duck = first_duck(game)
    duck.speed = 1.0
    duck.x = FIELD_WIDTH + DUCK_HALF_WIDTH + 1.0
    game.update(0.001)
    assert game.lives == START_LIVES - 1
    assert game.ducks == []
    assert not game.game_over


def test_three_escapes_end_the_game():
    game = Game(seed=9)
    game.ducks_to_spawn = 0
    for _ in range(START_LIVES):
        game.break_timer = 0.0
        game.ducks = [Duck(x=-DUCK_HALF_WIDTH - 1.0, y=10.0, base_y=10.0, speed=-1.0, age=0.0, phase=0.0)]
        game.update(0.001)
    assert game.game_over
    assert game.lives == 0

    score = game.score
    game.update(1.0)
    assert game.shoot(0.0, 0.0) is False
    assert game.score == score


def test_cleared_wave_leads_to_the_next_wave():
    game = Game(seed=11)
    game.ducks_to_spawn = 0
    game.ducks = []
    game.update(0.01)
    assert game.break_timer > 0

    game.update(WAVE_BREAK_SECONDS)
    assert game.wave == 2
    assert game.ducks_to_spawn == ducks_in_wave(2)
    assert game.break_timer == 0


def test_later_waves_are_faster():
    early, late = Game(seed=21), Game(seed=21)
    late.wave = 5
    early_duck, late_duck = first_duck(early), first_duck(late)
    assert ducks_in_wave(2) > ducks_in_wave(1)
    assert abs(late_duck.speed) > abs(early_duck.speed)


def test_ducks_fly_in_both_directions():
    game = Game(seed=1234)
    directions = set()
    for _ in range(40):
        game.ducks = []
        game.ducks_to_spawn = 1
        game.spawn_timer = 0.0
        game.update(0.001)
        directions.add(game.ducks[0].speed > 0)
    assert directions == {True, False}


def test_bobbing_keeps_ducks_inside_the_field():
    game = Game(seed=8)
    for _ in range(200):
        game.update(0.05)
    for duck in game.ducks:
        assert 0 < duck.y < 40
        assert math.isfinite(duck.x)
