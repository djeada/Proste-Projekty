/* Tests for the weather logic, using a saved wttr.in reply (no network needed). */
#include "weather.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_sample(void) {
    FILE *file = fopen(SAMPLE_JSON, "rb");
    assert(file != NULL);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *text = malloc((size_t)size + 1);
    assert(text != NULL);
    assert(fread(text, 1, (size_t)size, file) == (size_t)size);
    text[size] = '\0';
    fclose(file);
    return text;
}

static void test_url_plain_city(void) {
    char url[256];
    assert(weather_url(url, sizeof(url), "Warszawa") == 0);
    assert(strcmp(url, "https://wttr.in/Warszawa?format=j1") == 0);
}

static void test_url_encodes_spaces_and_polish_letters(void) {
    char url[256];
    assert(weather_url(url, sizeof(url), "Kraków Łódź") == 0);
    assert(strcmp(url, "https://wttr.in/Krak%C3%B3w%20%C5%81%C3%B3d%C5%BA?format=j1") == 0);
}

static void test_url_keeps_postal_code_dash(void) {
    char url[256];
    assert(weather_url(url, sizeof(url), "00-001") == 0);
    assert(strcmp(url, "https://wttr.in/00-001?format=j1") == 0);
}

static void test_url_too_small_buffer(void) {
    char url[16];
    assert(weather_url(url, sizeof(url), "Warszawa") == -1);
}

static void test_parse_current_conditions(void) {
    char *json = read_sample();
    Weather weather;
    assert(weather_parse(json, &weather) == 0);
    assert(weather.temp == 20);
    assert(weather.feels_like == 15);
    assert(strcmp(weather.description, "Overcast") == 0);
    assert(weather.humidity == 50);
    assert(weather.wind_speed == 22);
    assert(strcmp(weather.wind_dir, "SSW") == 0);
    assert(weather.pressure == 1005);
    assert(strcmp(weather.sunrise, "06:49 AM") == 0);
    assert(strcmp(weather.sunset, "05:58 PM") == 0);
    free(json);
}

static void test_parse_three_day_forecast(void) {
    char *json = read_sample();
    Weather weather;
    assert(weather_parse(json, &weather) == 0);
    assert(strcmp(weather.forecast[0].date, "2026-10-08") == 0);
    assert(weather.forecast[0].min_temp == 14);
    assert(weather.forecast[0].max_temp == 22);
    assert(strcmp(weather.forecast[0].description, "Overcast") == 0);
    assert(strcmp(weather.forecast[1].date, "2026-10-09") == 0);
    assert(weather.forecast[1].max_temp == 18);
    assert(strcmp(weather.forecast[2].date, "2026-10-10") == 0);
    assert(weather.forecast[2].min_temp == 12);
    assert(strcmp(weather.forecast[2].description, "Patchy rain nearby") == 0);
    free(json);
}

static void test_parse_rejects_non_json_reply(void) {
    Weather weather;
    assert(weather_parse("location not found", &weather) == -1);
    assert(weather_parse("", &weather) == -1);
}

static void test_parse_rejects_truncated_reply(void) {
    Weather weather;
    assert(weather_parse("{\"current_condition\": [{\"temp_C\": \"20\"", &weather) == -1);
}

static void test_report_contains_values(void) {
    char *json = read_sample();
    Weather weather;
    char report[4096];
    assert(weather_parse(json, &weather) == 0);
    weather_report(&weather, "Warszawa", report, sizeof(report));
    assert(strstr(report, "Weather in Warszawa") != NULL);
    assert(strstr(report, "20 °C") != NULL);
    assert(strstr(report, "feels like 15 °C") != NULL);
    assert(strstr(report, "Wind") != NULL);
    assert(strstr(report, "1005 hPa") != NULL);
    assert(strstr(report, "2026-10-10") != NULL);
    assert(strstr(report, "Patchy rain nearby") != NULL);
    free(json);
}

int main(void) {
    test_url_plain_city();
    test_url_encodes_spaces_and_polish_letters();
    test_url_keeps_postal_code_dash();
    test_url_too_small_buffer();
    test_parse_current_conditions();
    test_parse_three_day_forecast();
    test_parse_rejects_non_json_reply();
    test_parse_rejects_truncated_reply();
    test_report_contains_values();
    printf("All weather tests passed.\n");
    return 0;
}
