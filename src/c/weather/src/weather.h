/* Weather logic: build the wttr.in URL, parse its JSON reply, format the report. */
#ifndef WEATHER_H
#define WEATHER_H

#include <stddef.h>

#define WEATHER_DAYS 3
#define WEATHER_TEXT 64

typedef struct {
    char date[16];
    int min_temp;
    int max_temp;
    char description[WEATHER_TEXT];
} ForecastDay;

typedef struct {
    int temp;
    int feels_like;
    char description[WEATHER_TEXT];
    int humidity;
    int wind_speed;
    char wind_dir[8];
    int pressure;
    char sunrise[16];
    char sunset[16];
    ForecastDay forecast[WEATHER_DAYS];
} Weather;

/* Writes https://wttr.in/<city>?format=j1 with the city percent-encoded. Returns 0, or -1 if out is too small. */
int weather_url(char *out, size_t size, const char *city);

/* Fills weather from the JSON reply. Returns 0, or -1 if the reply has no weather data. */
int weather_parse(const char *json, Weather *weather);

/* Writes the colored text report into out (always NUL-terminated). */
void weather_report(const Weather *weather, const char *city, char *out, size_t size);

#endif /* WEATHER_H */
