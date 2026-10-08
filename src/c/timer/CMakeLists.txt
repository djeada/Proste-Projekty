cmake_minimum_required(VERSION 3.10)
project(timer C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(timer_logic STATIC src/timer.c)
target_include_directories(timer_logic PUBLIC src)

add_executable(timer src/main.c)
target_link_libraries(timer timer_logic)

enable_testing()
add_executable(test_timer tests/test_timer.c)
target_link_libraries(test_timer timer_logic)
add_test(NAME test_timer COMMAND test_timer)
