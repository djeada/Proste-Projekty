cmake_minimum_required(VERSION 3.10)
project(messenger C)

set(CMAKE_C_STANDARD 99)

find_package(Threads REQUIRED)

add_executable(main src/main.c src/messenger.c)
target_link_libraries(main Threads::Threads)

enable_testing()
add_executable(test_messenger tests/test_messenger.c src/messenger.c)
target_include_directories(test_messenger PRIVATE src)
add_test(NAME test_messenger COMMAND test_messenger)

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
