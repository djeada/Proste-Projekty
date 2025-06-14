#include "weather.h"
#include <stdio.h>
#include <string.h>

void weather_set(WeatherInfo *info, const char *city, float temperature, const char *description) {
    strncpy(info->city, city, MAX_CITY_LEN - 1);
    info->city[MAX_CITY_LEN - 1] = '\0';
    info->temperature = temperature;
    strncpy(info->description, description, MAX_DESC_LEN - 1);
    info->description[MAX_DESC_LEN - 1] = '\0';
}

void weather_print(const WeatherInfo *info) {
    printf("City: %s\n", info->city);
    printf("Temperature: %.1f C\n", info->temperature);
    printf("Description: %s\n", info->description);
}
