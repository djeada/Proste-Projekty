#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { MAX_CMD_LEN = 256, MAX_WEATHER_LEN = 256, MAX_CONDITION_LEN = 256 };

typedef struct {
    const char *condition;
} WeatherCondition;

void usage(const char *programName) {
    printf("Usage: %s [CITY]\n", programName);
    exit(EXIT_FAILURE);
}

void fetchWeatherData(const char *city, char *weatherData, size_t dataSize) {
    char command[MAX_CMD_LEN];
    snprintf(command, sizeof(command), "curl -s -L \"http://wttr.in/%s?format=3\"", city);
    FILE *filePointer = popen(command, "r");
    if (filePointer == NULL) {
        perror("Error executing curl command");
        exit(EXIT_FAILURE);
    }
    fgets(weatherData, (int)dataSize, filePointer);
    pclose(filePointer);
}

void printWeatherCondition(const char *weatherData, WeatherCondition condition) {
    if (strstr(condition.condition, "Clear")) {
        printf("\e[93m☀️ %s\e[0m\n", weatherData);
    } else if (strstr(condition.condition, "Rain") || strstr(condition.condition, "Drizzle")) {
        printf("\e[94m🌧️ %s\e[0m\n", weatherData);
    } else if (strstr(condition.condition, "Cloud")) {
        printf("\e[37m☁️ %s\e[0m\n", weatherData);
    } else if (strstr(condition.condition, "Snow")) {
        printf("\e[96m❄️ %s\e[0m\n", weatherData);
    } else {
        printf("\e[95m🌀 %s\e[0m\n", weatherData); // Default case
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage(argv[0]);
    }

    const char *city = argv[1];
    char weatherData[MAX_WEATHER_LEN];

    fetchWeatherData(city, weatherData, sizeof(weatherData));

    char conditionStr[MAX_CONDITION_LEN];
    sscanf(weatherData, "%*[^:]:%255[^\n]", conditionStr);
    WeatherCondition condition = { conditionStr };

    printf("\e[1m\e[95mCity: %s\e[0m\n", city);
    printf("-----------------------------------\n");

    printWeatherCondition(weatherData, condition);

    printf("-----------------------------------\n");

    return EXIT_SUCCESS;
}
