cmake_minimum_required(VERSION 3.10)
project(weather C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(weather_logic STATIC src/weather.c)
target_include_directories(weather_logic PUBLIC src)

add_executable(weather src/main.c)
target_link_libraries(weather weather_logic)

enable_testing()
add_executable(test_weather tests/test_weather.c)
target_link_libraries(test_weather weather_logic)
target_compile_definitions(test_weather PRIVATE SAMPLE_JSON="${CMAKE_CURRENT_SOURCE_DIR}/tests/warszawa.json")
add_test(NAME test_weather COMMAND test_weather)
