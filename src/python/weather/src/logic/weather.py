"""
Weather information logic.
"""
from dataclasses import dataclass
from typing import Dict


@dataclass
class WeatherInfo:
    """Represents weather information for a location."""

    city: str
    temperature: float
    description: str

    def to_string(self) -> str:
        """Get formatted weather information."""
        return (
            f"Weather for {self.city}:\n"
            f"  Temperature: {self.temperature:.1f}°C\n"
            f"  Description: {self.description}"
        )


class WeatherService:
    """
    Simple weather service with mock data.
    In a real application, this would fetch data from an API.
    """

    # Mock weather data for demonstration
    MOCK_DATA: Dict[str, WeatherInfo] = {
        "london": WeatherInfo(
            city="London", temperature=12.5, description="Cloudy with light rain"
        ),
        "paris": WeatherInfo(
            city="Paris", temperature=15.0, description="Partly cloudy"
        ),
        "new york": WeatherInfo(
            city="New York", temperature=18.5, description="Sunny"
        ),
        "tokyo": WeatherInfo(
            city="Tokyo", temperature=22.0, description="Clear skies"
        ),
        "sydney": WeatherInfo(
            city="Sydney", temperature=25.5, description="Warm and sunny"
        ),
        "berlin": WeatherInfo(
            city="Berlin", temperature=10.0, description="Overcast"
        ),
        "moscow": WeatherInfo(
            city="Moscow", temperature=-5.0, description="Snow showers"
        ),
        "dubai": WeatherInfo(
            city="Dubai", temperature=35.0, description="Hot and sunny"
        ),
    }

    def get_weather(self, city: str) -> WeatherInfo:
        """
        Get weather information for a city.

        :param city: City name
        :return: WeatherInfo object
        """
        city_lower = city.lower().strip()
        if city_lower in self.MOCK_DATA:
            return self.MOCK_DATA[city_lower]

        # Return default weather for unknown cities
        return WeatherInfo(
            city=city.title(),
            temperature=20.0,
            description="Weather data not available",
        )

    def list_cities(self) -> str:
        """Get list of available cities."""
        cities = [info.city for info in self.MOCK_DATA.values()]
        return ", ".join(sorted(cities))


def create_weather_info(
    city: str, temperature: float, description: str
) -> WeatherInfo:
    """
    Create a WeatherInfo object.

    :param city: City name
    :param temperature: Temperature in Celsius
    :param description: Weather description
    :return: WeatherInfo object
    """
    return WeatherInfo(city=city, temperature=temperature, description=description)
