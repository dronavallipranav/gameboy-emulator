# Add the directories where the compiler should look for additional include files
include_directories(${CUNIT_INCLUDE_DIRS})

# Add an executable target for the tests
add_executable(z80_tests
    test_arithmetic.c
)

# Link the test executable with the library and CUnit
target_link_libraries(z80_tests
    z80_lib
    ${CUNIT_LIBRARIES}
)

# Enable testing
enable_testing()

# Add the test to CTest's test list
add_test(NAME z80_tests COMMAND z80_tests)
