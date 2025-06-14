#ifndef WEATHER_H
#define WEATHER_H

#define MAX_CITY_LEN 64
#define MAX_DESC_LEN 128

typedef struct {
    char city[MAX_CITY_LEN];
    float temperature;
    char description[MAX_DESC_LEN];
} WeatherInfo;

void weather_print(const WeatherInfo *info);
void weather_set(WeatherInfo *info, const char *city, float temperature, const char *description);

#endif // WEATHER_H
