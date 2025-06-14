#include "../src/weather.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void test_weather_set_and_print() {
    WeatherInfo info;
    weather_set(&info, "London", 21.5, "Clear sky");
    assert(strcmp(info.city, "London") == 0);
    assert(info.temperature == 21.5f);
    assert(strcmp(info.description, "Clear sky") == 0);
    weather_print(&info);
}

int main() {
    test_weather_set_and_print();
    printf("Weather logic tests passed.\n");
    return 0;
}
