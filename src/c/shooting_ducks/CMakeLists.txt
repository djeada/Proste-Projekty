cmake_minimum_required(VERSION 3.10)
project(shooting_ducks C)

set(CMAKE_C_STANDARD 99)

find_package(Curses REQUIRED)

add_library(shooting_ducks_logic src/shooting_ducks.c)
target_include_directories(shooting_ducks_logic PUBLIC src)
target_link_libraries(shooting_ducks_logic ${CURSES_LIBRARIES})

add_executable(main src/main.c)
target_link_libraries(main shooting_ducks_logic ${CURSES_LIBRARIES})

# Enable testing
enable_testing()
add_executable(test_shooting_ducks tests/test_shooting_ducks.c)
target_link_libraries(test_shooting_ducks shooting_ducks_logic ${CURSES_LIBRARIES})
target_include_directories(test_shooting_ducks PRIVATE src)
add_test(NAME test_shooting_ducks COMMAND test_shooting_ducks)

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
