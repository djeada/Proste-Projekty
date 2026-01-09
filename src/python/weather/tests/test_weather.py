import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from weather import WeatherInfo, WeatherService, create_weather_info


class TestWeatherInfo(unittest.TestCase):
    def test_create_weather_info(self):
        info = create_weather_info("Test City", 20.5, "Sunny")
        self.assertEqual(info.city, "Test City")
        self.assertEqual(info.temperature, 20.5)
        self.assertEqual(info.description, "Sunny")

    def test_to_string(self):
        info = WeatherInfo(city="London", temperature=15.0, description="Cloudy")
        output = info.to_string()
        self.assertIn("London", output)
        self.assertIn("15.0", output)
        self.assertIn("Cloudy", output)


class TestWeatherService(unittest.TestCase):
    def test_get_weather_known_city(self):
        service = WeatherService()
        info = service.get_weather("London")
        self.assertEqual(info.city, "London")
        self.assertIsNotNone(info.temperature)
        self.assertIsNotNone(info.description)

    def test_get_weather_case_insensitive(self):
        service = WeatherService()
        info1 = service.get_weather("LONDON")
        info2 = service.get_weather("london")
        self.assertEqual(info1.city, info2.city)

    def test_get_weather_unknown_city(self):
        service = WeatherService()
        info = service.get_weather("Unknown City")
        self.assertEqual(info.city, "Unknown City")
        self.assertIn("not available", info.description)

    def test_list_cities(self):
        service = WeatherService()
        cities = service.list_cities()
        self.assertIn("London", cities)
        self.assertIn("Paris", cities)


if __name__ == "__main__":
    unittest.main()
