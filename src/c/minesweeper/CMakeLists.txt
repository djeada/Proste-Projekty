cmake_minimum_required(VERSION 3.10)
project(minesweeper C)

set(CMAKE_C_STANDARD 99)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(minesweeper_logic STATIC src/minesweeper.c)
target_include_directories(minesweeper_logic PUBLIC src)

add_executable(minesweeper src/main.c)
target_link_libraries(minesweeper minesweeper_logic)

enable_testing()
add_executable(test_minesweeper tests/test_minesweeper.c)
target_link_libraries(test_minesweeper minesweeper_logic)
add_test(NAME test_minesweeper COMMAND test_minesweeper)
