cmake_minimum_required(VERSION 3.10)
project(terminal_effects C)

set(CMAKE_C_STANDARD 99)

# Every effect is a standalone program in src/
set(EFFECTS
    donut
    mandelbrot_zoom
    game_of_life
    matrix_rain
    doom_fire
    spinning_cube
    plasma
    quicksort_visualizer
    maze_solver
    ray_tracer
    warp_starfield
)

enable_testing()
foreach(effect ${EFFECTS})
    add_executable(${effect} src/${effect}.c)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${effect} PRIVATE -Wall -Wextra)
    endif()
    target_link_libraries(${effect} m)
    # Smoke test: the animation has to run to the end and exit cleanly
    add_test(NAME ${effect} COMMAND ${effect})
    set_tests_properties(${effect} PROPERTIES TIMEOUT 60)
endforeach()

install(TARGETS ${EFFECTS} DESTINATION bin)
