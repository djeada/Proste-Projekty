cmake_minimum_required(VERSION 3.10)
project(zombie_apocalypse C)

set(CMAKE_C_STANDARD 99)

find_package(Curses REQUIRED)

add_library(zombie_apocalypse_logic src/zombie_apocalypse.c)
target_include_directories(zombie_apocalypse_logic PUBLIC src)
target_link_libraries(zombie_apocalypse_logic ${CURSES_LIBRARIES})

add_executable(main src/main.c)
target_link_libraries(main zombie_apocalypse_logic ${CURSES_LIBRARIES})

# Enable testing
enable_testing()
add_executable(test_zombie_apocalypse tests/test_zombie_apocalypse.c)
target_link_libraries(test_zombie_apocalypse zombie_apocalypse_logic ${CURSES_LIBRARIES})
target_include_directories(test_zombie_apocalypse PRIVATE src)
add_test(NAME test_zombie_apocalypse COMMAND test_zombie_apocalypse)

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
