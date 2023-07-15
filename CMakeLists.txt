cmake_minimum_required(VERSION 3.10)

# Set the project name
project(z80_emulator)

# Add the directories where the compiler should look for additional include files
include_directories(Instructions)

# Add the source files to a library
add_library(z80_lib
    cpu.c
    ${PROJECT_SOURCE_DIR}/Instructions/arithmetic.c
)
get_target_property(SOURCES z80_lib SOURCES)
message("z80_lib sources = ${SOURCES}")
# Add the test directory
add_subdirectory(tests)
