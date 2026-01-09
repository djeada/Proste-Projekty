cmake_minimum_required(VERSION 3.10)
project(tic_tac_toe C)

set(CMAKE_C_STANDARD 99)

add_executable(main src/main.c src/tic_tac_toe.c)

enable_testing()
add_executable(test_tic_tac_toe tests/test_tic_tac_toe.c src/tic_tac_toe.c)
target_include_directories(test_tic_tac_toe PRIVATE src)
add_test(NAME test_tic_tac_toe COMMAND test_tic_tac_toe)

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
