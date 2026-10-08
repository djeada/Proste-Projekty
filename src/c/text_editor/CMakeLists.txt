cmake_minimum_required(VERSION 3.10)
project(text_editor C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

find_package(Curses REQUIRED)
# The wide-character functions (get_wch) are in the ncursesw library.
find_library(NCURSESW_LIBRARY NAMES ncursesw)
if(NOT NCURSESW_LIBRARY)
    message(FATAL_ERROR "The ncursesw library was not found")
endif()

add_library(text_editor_logic STATIC src/text_editor.c)
target_include_directories(text_editor_logic PUBLIC src)

add_executable(text_editor src/main.c)
target_include_directories(text_editor PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(text_editor text_editor_logic ${NCURSESW_LIBRARY})

enable_testing()
add_executable(test_text_editor tests/test_text_editor.c)
target_link_libraries(test_text_editor text_editor_logic)
add_test(NAME test_text_editor COMMAND test_text_editor)
