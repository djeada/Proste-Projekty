cmake_minimum_required(VERSION 3.10)
project(game_2048 C)

set(CMAKE_C_STANDARD 99)

add_library(game_2048_logic src/game_2048.c)
target_include_directories(game_2048_logic PUBLIC src)

add_executable(main src/main.c)
target_link_libraries(main game_2048_logic)

# Enable testing
enable_testing()
add_executable(test_game_2048 tests/test_game_2048.c)
target_link_libraries(test_game_2048 game_2048_logic)
target_include_directories(test_game_2048 PRIVATE src)
add_test(NAME test_game_2048 COMMAND test_game_2048)

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
