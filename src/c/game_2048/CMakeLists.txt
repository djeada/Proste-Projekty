cmake_minimum_required(VERSION 3.10)
project(game_2048 C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(game_2048_logic STATIC src/game_2048.c)
target_include_directories(game_2048_logic PUBLIC src)

add_executable(game_2048 src/main.c)
target_link_libraries(game_2048 game_2048_logic)

enable_testing()
add_executable(test_game_2048 tests/test_game_2048.c)
target_link_libraries(test_game_2048 game_2048_logic)
add_test(NAME test_game_2048 COMMAND test_game_2048)

# The tests use assert(), which is compiled out when NDEBUG is set (Release builds).
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(test_game_2048 PRIVATE -UNDEBUG)
endif()
