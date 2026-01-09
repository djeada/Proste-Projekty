cmake_minimum_required(VERSION 3.10)
project(text_editor C)

set(CMAKE_C_STANDARD 99)

add_executable(main src/main.c src/text_editor.c)

enable_testing()
add_executable(test_text_editor tests/test_text_editor.c src/text_editor.c)
target_include_directories(test_text_editor PRIVATE src)
add_test(NAME test_text_editor COMMAND test_text_editor)

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
