cmake_minimum_required(VERSION 3.10)
project(graphics_editor C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

find_package(SDL2 REQUIRED)

add_library(graphics_editor_logic STATIC src/graphics_editor.c)
target_include_directories(graphics_editor_logic PUBLIC src)

add_executable(graphics_editor src/main.c)
target_link_libraries(graphics_editor graphics_editor_logic SDL2::SDL2)

enable_testing()
add_executable(test_graphics_editor tests/test_graphics_editor.c)
target_link_libraries(test_graphics_editor graphics_editor_logic)
add_test(NAME test_graphics_editor COMMAND test_graphics_editor)
