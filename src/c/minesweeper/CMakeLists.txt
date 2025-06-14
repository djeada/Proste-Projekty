cmake_minimum_required(VERSION 3.10)
project(minesweeper C)

set(CMAKE_C_STANDARD 99)

add_library(minesweeper_logic src/minesweeper.c)
target_include_directories(minesweeper_logic PUBLIC src)

add_executable(main src/main.c)
target_link_libraries(main minesweeper_logic)

# Enable testing
enable_testing()
add_executable(test_minesweeper tests/test_minesweeper.c)
target_link_libraries(test_minesweeper minesweeper_logic)
target_include_directories(test_minesweeper PRIVATE src)
add_test(NAME test_minesweeper COMMAND test_minesweeper)

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
