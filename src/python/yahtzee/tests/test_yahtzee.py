import pytest

from yahtzee import Category, Game, card_total, score_for, upper_bonus, upper_total


def always_six():
    return 6


def scripted(faces):
    iterator = iter(faces)
    return lambda: next(iterator)


@pytest.mark.parametrize(
    "dice, category, expected",
    [
        ([3, 3, 3, 5, 5], Category.FULL_HOUSE, 25),
        ([4, 4, 4, 4, 4], Category.FULL_HOUSE, 0),
        ([1, 2, 3, 4, 6], Category.SMALL_STRAIGHT, 30),
        ([1, 1, 2, 3, 3], Category.SMALL_STRAIGHT, 0),
        ([2, 3, 4, 5, 6], Category.LARGE_STRAIGHT, 40),
        ([1, 2, 3, 4, 6], Category.LARGE_STRAIGHT, 0),
        ([4, 4, 4, 4, 4], Category.YAHTZEE, 50),
        ([4, 4, 4, 4, 5], Category.YAHTZEE, 0),
        ([2, 2, 2, 5, 6], Category.THREE_OF_A_KIND, 17),
        ([2, 2, 3, 5, 6], Category.THREE_OF_A_KIND, 0),
        ([2, 2, 2, 2, 6], Category.FOUR_OF_A_KIND, 14),
        ([2, 2, 2, 3, 6], Category.FOUR_OF_A_KIND, 0),
        ([5, 5, 1, 2, 5], Category.FIVES, 15),
        ([3, 3, 1, 3, 6], Category.THREES, 9),
        ([6, 6, 5, 5, 4], Category.CHANCE, 26),
    ],
)
def test_score_for(dice, category, expected):
    assert score_for(dice, category) == expected


def test_upper_bonus_needs_63_points():
    card = [3, 6, 9, 12, 15, 18] + [None] * 7
    assert upper_total(card) == 63
    assert upper_bonus(card) == 35
    card[Category.SIXES] = 17
    assert upper_total(card) == 62
    assert upper_bonus(card) == 0


def test_card_total_adds_everything():
    card = [None] * 13
    assert card_total(card) == 0
    card[Category.SIXES] = 30
    card[Category.FULL_HOUSE] = 25
    card[Category.CHANCE] = 22
    assert card_total(card) == 77


def test_roll_keeps_held_dice():
    game = Game(1)
    game.roll(scripted([1, 2, 3, 4, 5]))
    assert game.dice == [1, 2, 3, 4, 5]
    assert game.toggle_hold(0)
    assert game.toggle_hold(2)
    game.roll(scripted([6, 6, 6]))
    assert game.dice == [1, 6, 3, 6, 6]


def test_at_most_three_rolls():
    game = Game(1)
    assert all(game.roll(always_six) for _ in range(3))
    assert not game.roll(always_six)
    assert game.rolls == 3


def test_must_roll_before_holding_or_choosing():
    game = Game(1)
    assert not game.toggle_hold(0)
    assert not game.choose(Category.CHANCE)
    assert game.cards[0][Category.CHANCE] is None


def test_choose_ends_turn_and_moves_to_next_player():
    game = Game(2)
    game.roll(always_six)
    assert game.choose(Category.SIXES)
    assert game.cards[0][Category.SIXES] == 30
    assert game.current == 1
    assert game.rolls == 0 and game.dice == [0] * 5 and not any(game.held)
    game.roll(always_six)
    assert game.choose(Category.CHANCE)
    assert game.current == 0
    assert game.round == 2


def test_category_only_once_per_player():
    game = Game(2)
    game.roll(always_six)
    assert game.choose(Category.YAHTZEE)
    game.roll(always_six)
    assert game.choose(Category.CHANCE)
    game.roll(always_six)
    assert not game.choose(Category.YAHTZEE)
    assert game.current == 0
    assert game.choose(Category.SIXES)


def test_game_lasts_thirteen_rounds():
    game = Game(2)
    for turn in range(2 * len(Category)):
        assert not game.is_over
        game.roll(always_six)
        assert game.choose(Category(turn // 2))
    assert game.is_over
    assert game.round == 14
    assert not game.roll(always_six)
    assert not game.choose(Category.CHANCE)


def test_winner_is_player_with_most_points():
    game = Game(3)
    game.cards[0][Category.CHANCE] = 20
    game.cards[1][Category.YAHTZEE] = 50
    game.cards[2][Category.CHANCE] = 30
    assert game.winner() == 1
    game.cards[2][Category.YAHTZEE] = 50
    assert game.winner() == 2
