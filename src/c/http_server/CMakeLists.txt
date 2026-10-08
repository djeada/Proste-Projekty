cmake_minimum_required(VERSION 3.10)
project(http_server C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(http_server_logic STATIC src/http_server.c)
target_include_directories(http_server_logic PUBLIC src)

add_executable(http_server src/main.c)
target_link_libraries(http_server http_server_logic)

enable_testing()
add_executable(test_http_server tests/test_http_server.c)
target_link_libraries(test_http_server http_server_logic)
target_compile_definitions(test_http_server PRIVATE PUBLIC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/public")
add_test(NAME test_http_server COMMAND test_http_server)
