"""Tests of the hangman rules."""

import random

import pytest

from hangman import MAX_MISSES, WORD_LIST, Entry, Game, GuessResult, GameStatus, random_entry

BANANA = Entry("Fruit", "banana")


def test_random_entry_is_repeatable_with_a_seed():
    assert random_entry(random.Random(7)) == random_entry(random.Random(7))
    assert random_entry(random.Random(7)) in WORD_LIST


def test_word_list_has_only_lowercase_words():
    for entry in WORD_LIST:
        assert entry.word.isalpha() and entry.word.islower()


def test_new_game_is_fully_masked():
    assert Game(Entry("Fruit", "lemon")).masked() == "_ _ _ _ _"


def test_hit_reveals_all_occurrences():
    game = Game(BANANA)
    assert game.guess("a") == GuessResult.HIT
    assert game.masked() == "_ a _ a _ a"
    assert game.misses == 0


def test_miss_costs_one_attempt():
    game = Game(BANANA)
    assert game.guess("z") == GuessResult.MISS
    assert game.misses == 1


def test_repeated_guess_is_ignored():
    game = Game(BANANA)
    game.guess("b")
    game.guess("q")
    assert game.guess("b") == GuessResult.REPEAT
    assert game.guess("q") == GuessResult.REPEAT
    assert game.misses == 1


@pytest.mark.parametrize("text", ["1", " ", "", "ab"])
def test_invalid_guess(text):
    game = Game(BANANA)
    assert game.guess(text) == GuessResult.INVALID
    assert game.misses == 0


def test_uppercase_guess_counts_as_lowercase():
    game = Game(BANANA)
    assert game.guess("B") == GuessResult.HIT
    assert game.guess("b") == GuessResult.REPEAT


def test_win_when_all_letters_are_guessed():
    game = Game(BANANA)
    for letter in "ban":
        game.guess(letter)
    assert game.status == GameStatus.WON


def test_still_playing_before_the_word_is_complete():
    game = Game(BANANA)
    game.guess("b")
    game.guess("a")
    assert game.status == GameStatus.PLAYING


def test_lose_after_max_misses():
    game = Game(BANANA)
    for letter in "cdefgh"[:MAX_MISSES - 1]:
        game.guess(letter)
    assert game.status == GameStatus.PLAYING
    game.guess("h")
    assert game.misses == MAX_MISSES
    assert game.status == GameStatus.LOST
