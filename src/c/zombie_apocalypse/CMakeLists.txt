cmake_minimum_required(VERSION 3.10)
project(zombie_apocalypse C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

find_package(Curses REQUIRED)

add_library(zombie_apocalypse_logic STATIC src/zombie_apocalypse.c)
target_include_directories(zombie_apocalypse_logic PUBLIC src)
target_link_libraries(zombie_apocalypse_logic m)

add_executable(zombie_apocalypse src/main.c)
target_include_directories(zombie_apocalypse PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(zombie_apocalypse zombie_apocalypse_logic ${CURSES_LIBRARIES})

enable_testing()
add_executable(test_zombie_apocalypse tests/test_zombie_apocalypse.c)
target_link_libraries(test_zombie_apocalypse zombie_apocalypse_logic)
add_test(NAME test_zombie_apocalypse COMMAND test_zombie_apocalypse)
