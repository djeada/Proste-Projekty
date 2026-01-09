cmake_minimum_required(VERSION 3.10)
project(shopping_cart C)

set(CMAKE_C_STANDARD 99)

add_executable(main src/main.c src/shopping_cart.c)

enable_testing()
add_executable(test_shopping_cart tests/test_shopping_cart.c src/shopping_cart.c)
target_include_directories(test_shopping_cart PRIVATE src)
target_link_libraries(test_shopping_cart m)
add_test(NAME test_shopping_cart COMMAND test_shopping_cart)

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
