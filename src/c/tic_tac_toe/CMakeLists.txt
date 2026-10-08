cmake_minimum_required(VERSION 3.10)
project(tic_tac_toe C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

find_package(Curses REQUIRED)

add_library(tic_tac_toe_logic STATIC src/tic_tac_toe.c)
target_include_directories(tic_tac_toe_logic PUBLIC src)

add_executable(tic_tac_toe src/main.c)
target_include_directories(tic_tac_toe PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(tic_tac_toe tic_tac_toe_logic ${CURSES_LIBRARIES})

enable_testing()
add_executable(test_tic_tac_toe tests/test_tic_tac_toe.c)
target_link_libraries(test_tic_tac_toe tic_tac_toe_logic)
add_test(NAME test_tic_tac_toe COMMAND test_tic_tac_toe)
