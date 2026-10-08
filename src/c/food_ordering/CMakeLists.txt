cmake_minimum_required(VERSION 3.10)
project(food_ordering C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(food_ordering_logic STATIC src/food_ordering.c)
target_include_directories(food_ordering_logic PUBLIC src)

add_executable(food_ordering src/main.c)
target_link_libraries(food_ordering food_ordering_logic)

enable_testing()
add_executable(test_food_ordering tests/test_food_ordering.c)
target_link_libraries(test_food_ordering food_ordering_logic)
add_test(NAME test_food_ordering COMMAND test_food_ordering)
