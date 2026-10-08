/* Weather logic: URL building, JSON key lookup and report formatting. */
#include "weather.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOLD_CYAN "\033[1;36m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

enum {
    PATTERN_SIZE = 64,
    NUMBER_SIZE = 32,
    NOON_SLOT = 4 /* wttr.in gives eight 3-hour slots per day; index 4 is 12:00 */
};

/* Returns a pointer just after the first "key" at or after from, or NULL. */
static const char *find_key(const char *from, const char *key) {
    char pattern[PATTERN_SIZE];
    if (from == NULL) {
        return NULL;
    }
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);
    const char *found = strstr(from, pattern);
    return found ? found + strlen(pattern) : NULL;
}

/* Copies the quoted value that follows a key (the text after the key) and trims trailing spaces. */
static int read_text(const char *after_key, char *out, size_t size) {
    const char *colon = after_key ? strchr(after_key, ':') : NULL;
    const char *quote = colon ? strchr(colon, '"') : NULL;
    if (quote == NULL) {
        return -1;
    }
    size_t len = strcspn(quote + 1, "\"");
    if (len >= size) {
        len = size - 1;
    }
    memcpy(out, quote + 1, len);
    while (len > 0 && isspace((unsigned char)out[len - 1])) {
        len--;
    }
    out[len] = '\0';
    return 0;
}

static int get_text(const char *from, const char *key, char *out, size_t size) {
    return read_text(find_key(from, key), out, size);
}

static int get_int(const char *from, const char *key, int *out) {
    char buffer[NUMBER_SIZE];
    if (get_text(from, key, buffer, sizeof(buffer)) != 0) {
        return -1;
    }
    *out = atoi(buffer);
    return 0;
}

int weather_url(char *out, size_t size, const char *city) {
    static const char base[] = "https://wttr.in/";
    static const char suffix[] = "?format=j1";
    size_t used = sizeof(base) - 1;

    if (size < used + sizeof(suffix)) {
        return -1;
    }
    memcpy(out, base, used);
    for (const unsigned char *ch = (const unsigned char *)city; *ch != '\0'; ch++) {
        if (isalnum(*ch) || strchr("-._~", *ch) != NULL) {
            if (used + 2 > size) {
                return -1;
            }
            out[used++] = (char)*ch;
        } else {
            if (used + 4 > size) {
                return -1;
            }
            used += (size_t)snprintf(out + used, size - used, "%%%02X", (unsigned)*ch);
        }
    }
    if (used + sizeof(suffix) > size) {
        return -1;
    }
    memcpy(out + used, suffix, sizeof(suffix));
    return 0;
}

int weather_parse(const char *json, Weather *weather) {
    const char *current = find_key(json, "current_condition");
    if (current == NULL) {
        return -1;
    }
    if (get_int(current, "temp_C", &weather->temp) != 0 ||
        get_int(current, "FeelsLikeC", &weather->feels_like) != 0 ||
        get_int(current, "humidity", &weather->humidity) != 0 ||
        get_int(current, "windspeedKmph", &weather->wind_speed) != 0 ||
        get_int(current, "pressure", &weather->pressure) != 0 ||
        get_text(current, "winddir16Point", weather->wind_dir, sizeof(weather->wind_dir)) != 0 ||
        get_text(find_key(current, "weatherDesc"), "value", weather->description,
                 sizeof(weather->description)) != 0) {
        return -1;
    }

    const char *days = find_key(json, "weather");
    if (get_text(days, "sunrise", weather->sunrise, sizeof(weather->sunrise)) != 0 ||
        get_text(days, "sunset", weather->sunset, sizeof(weather->sunset)) != 0) {
        return -1;
    }

    const char *cursor = days;
    for (int i = 0; i < WEATHER_DAYS; i++) {
        ForecastDay *day = &weather->forecast[i];
        const char *date = find_key(cursor, "date");
        if (read_text(date, day->date, sizeof(day->date)) != 0 ||
            get_int(date, "mintempC", &day->min_temp) != 0 ||
            get_int(date, "maxtempC", &day->max_temp) != 0) {
            return -1;
        }
        const char *slot = find_key(date, "hourly");
        for (int slot_index = 0; slot_index <= NOON_SLOT && slot != NULL; slot_index++) {
            slot = find_key(slot, "weatherDesc");
        }
        if (read_text(find_key(slot, "value"), day->description, sizeof(day->description)) != 0) {
            return -1;
        }
        cursor = slot;
    }
    return 0;
}

/* Appends formatted text at *used, never writing past size - 1. */
static void append(char *out, size_t size, size_t *used, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int needed = vsnprintf(out + *used, size - *used, format, args);
    va_end(args);
    if (needed > 0 && *used < size - 1) {
        size_t room = size - *used - 1;
        *used += (size_t)needed < room ? (size_t)needed : room;
    }
}

void weather_report(const Weather *weather, const char *city, char *out, size_t size) {
    size_t used = 0;
    out[0] = '\0';
    append(out, size, &used, BOLD_CYAN "Weather in %s" RESET "\n", city);
    append(out, size, &used, "  Conditions   %s\n", weather->description);
    append(out, size, &used, "  Temperature  " YELLOW "%d °C" RESET " (feels like %d °C)\n",
           weather->temp, weather->feels_like);
    append(out, size, &used, "  Humidity     %d %%\n", weather->humidity);
    append(out, size, &used, "  Wind         %d km/h %s\n", weather->wind_speed, weather->wind_dir);
    append(out, size, &used, "  Pressure     %d hPa\n", weather->pressure);
    append(out, size, &used, "  Sunrise      %s   Sunset %s\n", weather->sunrise, weather->sunset);
    append(out, size, &used, "\n" BOLD_CYAN "Forecast" RESET "\n");
    for (int i = 0; i < WEATHER_DAYS; i++) {
        const ForecastDay *day = &weather->forecast[i];
        append(out, size, &used, "  %s  " YELLOW "%3d / %3d °C" RESET "  %s\n", day->date,
               day->min_temp, day->max_temp, day->description);
    }
}
