cmake_minimum_required(VERSION 3.10)
project(shooting_ducks C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

find_package(Curses REQUIRED)

add_library(shooting_ducks_logic STATIC src/shooting_ducks.c)
target_include_directories(shooting_ducks_logic PUBLIC src)
target_link_libraries(shooting_ducks_logic m)

add_executable(shooting_ducks src/main.c)
target_include_directories(shooting_ducks PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(shooting_ducks shooting_ducks_logic ${CURSES_LIBRARIES})

enable_testing()
add_executable(test_shooting_ducks tests/test_shooting_ducks.c)
target_link_libraries(test_shooting_ducks shooting_ducks_logic)
add_test(NAME test_shooting_ducks COMMAND test_shooting_ducks)
