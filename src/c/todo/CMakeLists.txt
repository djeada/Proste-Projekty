cmake_minimum_required(VERSION 3.10)
project(todo C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(todo_logic STATIC src/todo.c)
target_include_directories(todo_logic PUBLIC src)

add_executable(todo src/main.c)
target_link_libraries(todo todo_logic)

enable_testing()
add_executable(test_todo tests/test_todo.c)
target_link_libraries(test_todo todo_logic)
add_test(NAME test_todo COMMAND test_todo)
