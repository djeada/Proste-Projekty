cmake_minimum_required(VERSION 3.10)
project(timer C)

set(CMAKE_C_STANDARD 99)

add_library(timer_logic src/timer.c)
target_include_directories(timer_logic PUBLIC src)

add_executable(main src/main.c)
target_link_libraries(main timer_logic)

# Enable testing
enable_testing()
add_executable(test_timer tests/test_timer.c)
target_link_libraries(test_timer timer_logic)
target_include_directories(test_timer PRIVATE src)
add_test(NAME test_timer COMMAND test_timer)

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
