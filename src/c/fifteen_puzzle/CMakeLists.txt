cmake_minimum_required(VERSION 3.10)
project(fifteen_puzzle C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(fifteen_puzzle_logic STATIC src/fifteen_puzzle.c)
target_include_directories(fifteen_puzzle_logic PUBLIC src)

find_package(Curses REQUIRED)

add_executable(fifteen_puzzle src/main.c)
target_include_directories(fifteen_puzzle PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(fifteen_puzzle fifteen_puzzle_logic ${CURSES_LIBRARIES})

enable_testing()
add_executable(test_fifteen_puzzle tests/test_fifteen_puzzle.c)
target_link_libraries(test_fifteen_puzzle fifteen_puzzle_logic)
add_test(NAME test_fifteen_puzzle COMMAND test_fifteen_puzzle)
