cmake_minimum_required(VERSION 3.10)
project(calculator C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(calculator_logic STATIC src/calculator.c)
target_include_directories(calculator_logic PUBLIC src)

add_executable(calculator src/main.c)
target_link_libraries(calculator calculator_logic)

enable_testing()
add_executable(test_calculator tests/test_calculator.c)
target_link_libraries(test_calculator calculator_logic)
add_test(NAME test_calculator COMMAND test_calculator)
