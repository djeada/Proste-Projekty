cmake_minimum_required(VERSION 3.10)
project(projekt_c VERSION 1.0)

add_executable(projekt_c src/main.c src/hello.c)

# Enable testing
include(CTest)
enable_testing()

# Add test(s)
add_executable(test_hello tests/test_hello.c src/hello.c)
target_include_directories(test_hello PRIVATE src)
add_test(NAME test_hello COMMAND test_hello)

# Install target
install(TARGETS main DESTINATION bin)

# Lint and format targets
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
