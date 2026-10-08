cmake_minimum_required(VERSION 3.10)
project(terminal_effects C)

set(CMAKE_C_STANDARD 99)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(EFFECTS
    donut
    mandelbrot_zoom
    game_of_life
    matrix_rain
    fire
    spinning_cube
    plasma
    quicksort_visualizer
    maze_solver
    ray_tracer
    warp_starfield
    langtons_ant
    fireworks
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -ffp-contract=off)
endif()

add_library(term STATIC src/term.c)
target_include_directories(term PUBLIC src)

enable_testing()
foreach(effect ${EFFECTS})
    add_executable(${effect} src/${effect}.c)
    target_link_libraries(${effect} term m)
    add_test(NAME ${effect} COMMAND ${effect} 50)
    set_tests_properties(${effect} PROPERTIES ENVIRONMENT NO_SLEEP=1 TIMEOUT 30)
endforeach()

