cmake_minimum_required(VERSION 3.10)
project(hangman C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(hangman_logic STATIC src/hangman.c)
target_include_directories(hangman_logic PUBLIC src)

add_executable(hangman src/main.c)
target_link_libraries(hangman hangman_logic)

enable_testing()
add_executable(test_hangman tests/test_hangman.c)
target_link_libraries(test_hangman hangman_logic)
add_test(NAME test_hangman COMMAND test_hangman)
