cmake_minimum_required(VERSION 3.10)
project(messenger C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(messenger_logic STATIC src/messenger.c)
target_include_directories(messenger_logic PUBLIC src)

add_executable(messenger src/main.c)
target_link_libraries(messenger messenger_logic)

enable_testing()
add_executable(test_messenger tests/test_messenger.c)
target_link_libraries(test_messenger messenger_logic)
add_test(NAME test_messenger COMMAND test_messenger)
