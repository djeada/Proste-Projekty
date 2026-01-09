cmake_minimum_required(VERSION 3.10)
project(graphics_editor C)

set(CMAKE_C_STANDARD 99)

add_executable(main src/main.c src/graphics_editor.c)
target_link_libraries(main m)

enable_testing()
add_executable(test_graphics_editor tests/test_graphics_editor.c src/graphics_editor.c)
target_include_directories(test_graphics_editor PRIVATE src)
target_link_libraries(test_graphics_editor m)
add_test(NAME test_graphics_editor COMMAND test_graphics_editor)

install(TARGETS main DESTINATION bin)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set_target_properties(main PROPERTIES C_CLANG_TIDY "clang-tidy")
endif()
find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND clang-format -i src/*.c src/*.h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()
