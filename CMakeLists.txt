cmake_minimum_required(VERSION 3.10)

# Set the project name
project(z80_emulator)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

# Add the directories where the compiler should look for additional include files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions)

# Add the source files to a library
add_library(z80_lib
    src/cpu.c
    ${PROJECT_SOURCE_DIR}/src/Instructions/arithmetic.c
)


# Add the test directory
add_subdirectory(tests)
