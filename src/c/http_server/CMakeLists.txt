cmake_minimum_required(VERSION 3.10)
project(http_server C)

set(CMAKE_C_STANDARD 99)

add_executable(main src/main.c)

install(TARGETS main DESTINATION bin)

enable_testing()
add_test(NAME test_http_server COMMAND bash ${CMAKE_SOURCE_DIR}/tests/test_http_server.sh)

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
