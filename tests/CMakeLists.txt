# Specify where the CUnit headers are
include_directories(/opt/homebrew/Cellar/cunit/2.1-3/include)
# Specify where the CUnit libraries are
link_directories(/opt/homebrew/Cellar/cunit/2.1-3/lib)

# Add the directories where the compiler should look for additional include files
include_directories(../src/Instructions)
include_directories(../include)


# Add the test executable
add_executable(z80_tests
    test_suite.c
)

# Link the test executable with the library and CUnit
target_link_libraries(z80_tests
    z80_lib
    cunit  # CUnit library
)

