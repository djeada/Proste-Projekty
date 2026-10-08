"""Weather logic: build the wttr.in URL, parse its JSON reply and format the report."""

import json
from dataclasses import dataclass
from typing import List
from urllib.parse import quote

RESET = "\033[0m"
BOLD_CYAN = "\033[1;36m"
YELLOW = "\033[33m"
FORECAST_DAYS = 3
NOON_SLOT = 4  # wttr.in gives eight 3-hour slots per day; index 4 is 12:00


@dataclass
class ForecastDay:
    date: str
    min_temp: int
    max_temp: int
    description: str


@dataclass
class Weather:
    temp: int
    feels_like: int
    description: str
    humidity: int
    wind_speed: int
    wind_dir: str
    pressure: int
    sunrise: str
    sunset: str
    forecast: List[ForecastDay]


def build_url(city: str) -> str:
    # safe="" also encodes "/", so a city name cannot change the path
    return f"https://wttr.in/{quote(city, safe='')}?format=j1"


def parse_weather(text: str) -> Weather:
    """Read the wttr.in JSON reply. Raises ValueError if it has no weather data."""
    try:
        data = json.loads(text)
        current = data["current_condition"][0]
        days = data["weather"]
        forecast = [
            ForecastDay(
                date=day["date"],
                min_temp=int(day["mintempC"]),
                max_temp=int(day["maxtempC"]),
                description=day["hourly"][NOON_SLOT]["weatherDesc"][0]["value"].strip(),
            )
            for day in days[:FORECAST_DAYS]
        ]
        return Weather(
            temp=int(current["temp_C"]),
            feels_like=int(current["FeelsLikeC"]),
            description=current["weatherDesc"][0]["value"].strip(),
            humidity=int(current["humidity"]),
            wind_speed=int(current["windspeedKmph"]),
            wind_dir=current["winddir16Point"],
            pressure=int(current["pressure"]),
            sunrise=days[0]["astronomy"][0]["sunrise"],
            sunset=days[0]["astronomy"][0]["sunset"],
            forecast=forecast,
        )
    except (KeyError, IndexError, TypeError, ValueError) as error:
        raise ValueError("unexpected weather data") from error


def format_report(weather: Weather, city: str) -> str:
    lines = [
        f"{BOLD_CYAN}Weather in {city}{RESET}",
        f"  Conditions   {weather.description}",
        f"  Temperature  {YELLOW}{weather.temp} °C{RESET} (feels like {weather.feels_like} °C)",
        f"  Humidity     {weather.humidity} %",
        f"  Wind         {weather.wind_speed} km/h {weather.wind_dir}",
        f"  Pressure     {weather.pressure} hPa",
        f"  Sunrise      {weather.sunrise}   Sunset {weather.sunset}",
        "",
        f"{BOLD_CYAN}Forecast{RESET}",
    ]
    for day in weather.forecast:
        lines.append(
            f"  {day.date}  {YELLOW}{day.min_temp:3d} / {day.max_temp:3d} °C{RESET}  {day.description}"
        )
    return "\n".join(lines)
