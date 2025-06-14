cmake_minimum_required(VERSION 3.10)
project(snake C)

set(CMAKE_C_STANDARD 99)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_library(snake_logic src/snake.c)
target_include_directories(snake_logic PUBLIC src)

add_executable(main src/main.c)
target_link_libraries(main snake_logic ${CURSES_LIBRARIES})

# Enable testing
enable_testing()
add_executable(test_snake tests/test_snake.c)
target_link_libraries(test_snake snake_logic ${CURSES_LIBRARIES})
target_include_directories(test_snake PRIVATE src)
add_test(NAME test_snake COMMAND test_snake)

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
