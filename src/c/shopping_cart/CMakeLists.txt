cmake_minimum_required(VERSION 3.10)
project(shopping_cart C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(shopping_cart_logic STATIC src/shopping_cart.c)
target_include_directories(shopping_cart_logic PUBLIC src)

add_executable(shopping_cart src/main.c)
target_link_libraries(shopping_cart shopping_cart_logic)

enable_testing()
add_executable(test_shopping_cart tests/test_shopping_cart.c)
target_link_libraries(test_shopping_cart shopping_cart_logic)
add_test(NAME test_shopping_cart COMMAND test_shopping_cart)
