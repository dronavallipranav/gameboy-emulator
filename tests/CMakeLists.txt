
include_directories(/opt/homebrew/Cellar/cunit/2.1-3/include)

link_directories(/opt/homebrew/Cellar/cunit/2.1-3/lib)


include_directories(../src/Instructions)
include_directories(../include)



add_executable(z80_tests
    test_suite.c
)


target_link_libraries(z80_tests
    z80_lib
    cunit  
)

