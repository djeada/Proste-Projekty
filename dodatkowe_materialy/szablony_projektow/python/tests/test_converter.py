import pytest

from converter import celsius_to_fahrenheit, convert, fahrenheit_to_celsius


def test_freezing_and_boiling_points():
    assert celsius_to_fahrenheit(0) == 32
    assert celsius_to_fahrenheit(100) == 212


def test_minus_forty_is_the_same_in_both_scales():
    assert fahrenheit_to_celsius(-40) == -40


def test_convert_returns_the_other_unit():
    assert convert(37, "c") == (pytest.approx(98.6), "F")
    assert convert(212, "F") == (pytest.approx(100), "C")


def test_unknown_unit_is_rejected():
    with pytest.raises(ValueError):
        convert(10, "K")
