cmake_minimum_required(VERSION 3.10)
project(caesar_cipher C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(caesar_cipher_logic STATIC src/caesar_cipher.c)
target_include_directories(caesar_cipher_logic PUBLIC src)

add_executable(caesar_cipher src/main.c)
target_link_libraries(caesar_cipher caesar_cipher_logic)

enable_testing()
add_executable(test_caesar_cipher tests/test_caesar_cipher.c)
target_link_libraries(test_caesar_cipher caesar_cipher_logic)
add_test(NAME test_caesar_cipher COMMAND test_caesar_cipher)
