cmake_minimum_required(VERSION 3.10)
project(todo C)

set(CMAKE_C_STANDARD 99)

add_library(todo_logic src/todo.c)
target_include_directories(todo_logic PUBLIC src)

add_executable(main src/main.c)
target_link_libraries(main todo_logic sqlite3)

# Enable testing
enable_testing()
add_executable(test_todo tests/test_todo.c)
target_link_libraries(test_todo todo_logic)
target_include_directories(test_todo PRIVATE src)
add_test(NAME test_todo COMMAND test_todo)

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
