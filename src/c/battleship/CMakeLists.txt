cmake_minimum_required(VERSION 3.10)
project(battleship C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(battleship_logic STATIC src/battleship.c)
target_include_directories(battleship_logic PUBLIC src)

find_package(Curses REQUIRED)
add_executable(battleship src/main.c)
target_include_directories(battleship PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(battleship battleship_logic ${CURSES_LIBRARIES})

enable_testing()
add_executable(test_battleship tests/test_battleship.c)
target_link_libraries(test_battleship battleship_logic)
add_test(NAME test_battleship COMMAND test_battleship)
