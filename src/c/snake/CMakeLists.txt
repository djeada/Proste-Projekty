cmake_minimum_required(VERSION 3.10)
project(snake C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

find_package(Curses REQUIRED)

add_library(snake_logic STATIC src/snake.c)
target_include_directories(snake_logic PUBLIC src)

add_executable(snake src/main.c)
target_include_directories(snake PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(snake snake_logic ${CURSES_LIBRARIES})

enable_testing()
add_executable(test_snake tests/test_snake.c)
target_link_libraries(test_snake snake_logic)
add_test(NAME test_snake COMMAND test_snake)
