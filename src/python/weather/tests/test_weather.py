"""Tests for the weather logic, using a saved wttr.in reply (no network needed)."""

from pathlib import Path

import pytest

from weather import ForecastDay, build_url, format_report, parse_weather

SAMPLE = (Path(__file__).parent / "warszawa.json").read_text(encoding="utf-8")


def test_build_url_plain_city():
    assert build_url("Warszawa") == "https://wttr.in/Warszawa?format=j1"


def test_build_url_encodes_spaces_and_polish_letters():
    assert build_url("Kraków Łódź") == "https://wttr.in/Krak%C3%B3w%20%C5%81%C3%B3d%C5%BA?format=j1"


def test_build_url_keeps_postal_code_dash_and_encodes_slash():
    assert build_url("00-001") == "https://wttr.in/00-001?format=j1"
    assert build_url("a/b") == "https://wttr.in/a%2Fb?format=j1"


def test_parse_current_conditions():
    weather = parse_weather(SAMPLE)
    assert weather.temp == 20
    assert weather.feels_like == 15
    assert weather.description == "Overcast"
    assert weather.humidity == 50
    assert weather.wind_speed == 22
    assert weather.wind_dir == "SSW"
    assert weather.pressure == 1005
    assert weather.sunrise == "06:49 AM"
    assert weather.sunset == "05:58 PM"


def test_parse_three_day_forecast():
    forecast = parse_weather(SAMPLE).forecast
    assert forecast == [
        ForecastDay("2026-10-08", 14, 22, "Overcast"),
        ForecastDay("2026-10-09", 11, 18, "Overcast"),
        ForecastDay("2026-10-10", 12, 14, "Patchy rain nearby"),
    ]


@pytest.mark.parametrize("text", ["", "location not found", '{"current_condition": [{"temp_C": "20"'])
def test_parse_rejects_bad_reply(text):
    with pytest.raises(ValueError):
        parse_weather(text)


def test_format_report_contains_values():
    report = format_report(parse_weather(SAMPLE), "Warszawa")
    assert "Weather in Warszawa" in report
    assert "20 °C" in report
    assert "feels like 15 °C" in report
    assert "1005 hPa" in report
    assert "2026-10-10" in report
    assert "Patchy rain nearby" in report
