cmake_minimum_required(VERSION 3.10)
project(version_control C)

set(CMAKE_C_STANDARD 99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

add_library(version_control_logic STATIC src/version_control.c)
target_include_directories(version_control_logic PUBLIC src)

add_executable(version_control src/main.c)
target_link_libraries(version_control version_control_logic)

enable_testing()
add_executable(test_version_control tests/test_version_control.c)
target_link_libraries(test_version_control version_control_logic)
add_test(NAME test_version_control COMMAND test_version_control)
