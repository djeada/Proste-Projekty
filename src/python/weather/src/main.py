"""
Python implementation of a weather information application.
"""
from src.python.weather.src.logic.weather import WeatherService


def main() -> None:
    service = WeatherService()

    print("Weather Information Application")
    print(f"Available cities: {service.list_cities()}")
    print("Commands: weather <city>, list, quit")
    print()

    while True:
        try:
            user_input = input("> ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\nGoodbye!")
            break

        if not user_input:
            continue

        parts = user_input.split(maxsplit=1)
        command = parts[0].lower()

        if command == "quit" or command == "q":
            print("Goodbye!")
            break
        elif command == "list" or command == "ls":
            print(f"Available cities: {service.list_cities()}")
        elif command == "weather" or command == "w":
            if len(parts) < 2:
                print("Usage: weather <city>")
            else:
                city = parts[1]
                info = service.get_weather(city)
                print(info.to_string())
        else:
            # Assume it's a city name
            info = service.get_weather(user_input)
            print(info.to_string())


if __name__ == "__main__":
    main()
