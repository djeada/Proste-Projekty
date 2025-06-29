cmake_minimum_required(VERSION 3.10)
project(calculator C)

set(CMAKE_C_STANDARD 99)

# Main calculator executable with both simple and advanced modes
add_executable(calculator src/main.c src/calculator.c src/parser.c src/repl.c)
target_link_libraries(calculator m)

# Test executable
enable_testing()
add_executable(test_calculator tests/test_calculator.c src/calculator.c src/parser.c src/repl.c)
target_include_directories(test_calculator PRIVATE src)
target_link_libraries(test_calculator m)
add_test(NAME test_calculator COMMAND test_calculator)

# Install the calculator binary
install(TARGETS calculator DESTINATION bin)

# Clang-tidy integration
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set_target_properties(calculator PROPERTIES C_CLANG_TIDY "clang-tidy")
endif()

# Clang-format integration
find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND clang-format -i src/*.c src/*.h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()
