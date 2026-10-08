cmake_minimum_required(VERSION 3.10)
project(converter C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(converter_logic STATIC src/converter.c)
target_include_directories(converter_logic PUBLIC src)

add_executable(converter src/main.c)
target_link_libraries(converter converter_logic)

enable_testing()
add_executable(test_converter tests/test_converter.c)
target_link_libraries(test_converter converter_logic m)
add_test(NAME test_converter COMMAND test_converter)
