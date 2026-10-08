"""User interface: asks for a city, downloads its weather from wttr.in and prints the report."""

import sys
import urllib.error
import urllib.request

from weather import build_url, format_report, parse_weather

TIMEOUT_SECONDS = 10


def ask_city() -> str:
    if len(sys.argv) > 1:
        return " ".join(sys.argv[1:]).strip()
    try:
        return input("City or postal code: ").strip()
    except EOFError:
        return ""


def fetch(city: str) -> str:
    with urllib.request.urlopen(build_url(city), timeout=TIMEOUT_SECONDS) as response:
        return response.read().decode("utf-8")


def main() -> None:
    city = ask_city()
    if not city:
        sys.exit("Please enter a city name or postal code.")
    try:
        text = fetch(city)
    except urllib.error.HTTPError as error:  # wttr.in answers HTTP 500 for unknown places
        sys.exit(f"City not found: {city} (HTTP {error.code})")
    except OSError as error:  # no network, DNS failure, timeout
        sys.exit(f"Network error: could not reach wttr.in ({error})")
    try:
        weather = parse_weather(text)
    except ValueError:
        sys.exit(f"Empty or unexpected response from wttr.in for: {city}")
    print(format_report(weather, city))


if __name__ == "__main__":
    main()
