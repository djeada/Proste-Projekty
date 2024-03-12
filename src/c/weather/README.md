# Weather Fetcher in C
This is a simple C program that fetches and displays weather information for a specified city using the wttr.in service. The program uses command line arguments to take a city name and then fetches the current weather condition for that city, displaying it along with a relevant emoji.

## How to Use
- Run the program with a city name as the argument.
- The program will display the current weather condition for the specified city.
- Weather conditions are accompanied by emojis for a more visual representation.

## Installation

### Compiling the Program
To compile the program, follow these steps:
1. Clone or download the repository to your local machine.
2. Navigate to the directory containing the `weather_fetcher.c` file.
3. Compile the program using GCC:

```
gcc weather_fetcher.c -o weather_fetcher
```

4. Run the program with a city name:

```
./weather_fetcher [City Name]
```

For example:

```
./weather_fetcher London
```

## Customization
- The program uses the wttr.in service for fetching weather data. You can modify the program to use a different API or service if desired.
- The `printWeatherCondition` function can be customized to display different emojis or information based on the weather condition.
