#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(const char *programName) {
    printf("Usage: %s [CITY]\n", programName);
    exit(EXIT_FAILURE);
}

void fetchWeatherData(const char *city, char *weatherData, size_t dataSize) {
    char command[256];
    snprintf(command, sizeof(command), "curl -s -L \"http://wttr.in/%s?format=3\"", city);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error executing curl command");
        exit(EXIT_FAILURE);
    }
    fgets(weatherData, dataSize, fp);
    pclose(fp);
}

void printWeatherCondition(const char *condition, const char *weatherData) {
    if (strstr(condition, "Clear")) {
        printf("\e[93m☀️ %s\e[0m\n", weatherData);
    } else if (strstr(condition, "Rain") || strstr(condition, "Drizzle")) {
        printf("\e[94m🌧️ %s\e[0m\n", weatherData);
    } else if (strstr(condition, "Cloud")) {
        printf("\e[37m☁️ %s\e[0m\n", weatherData);
    } else if (strstr(condition, "Snow")) {
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
    char weatherData[256];

    fetchWeatherData(city, weatherData, sizeof(weatherData));

    char condition[256];
    sscanf(weatherData, "%*[^:]:%255[^\n]", condition);

    printf("\e[1m\e[95mCity: %s\e[0m\n", city);
    printf("-----------------------------------\n");

    printWeatherCondition(condition, weatherData);

    printf("-----------------------------------\n");

    return EXIT_SUCCESS;
}
