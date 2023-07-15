cmake_minimum_required(VERSION 3.10)

# Set the project name
project(z80_emulator)

# Add the directories where the compiler should look for additional include files
include_directories(include)

# Add the source files to a library
add_library(z80_lib
    src/arithmetic.c
)

# Add an executable target that will be built from the source files
add_executable(z80_emulator
    src/main.c
)

# Link the executable target with the library
target_link_libraries(z80_emulator
    z80_lib
)

# Add the test directory
add_subdirectory(tests)
