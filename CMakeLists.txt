cmake_minimum_required(VERSION 3.10)

project(z80_emulator)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

#Link openGL library based on OS
add_executable(SDL2Test src/main.c)
if(APPLE)
    target_link_libraries(SDL2Test ${SDL2_LIBRARIES} z80_lib "-framework OpenGL")
else()
    target_link_libraries(SDL2Test ${SDL2_LIBRARIES} z80_lib GL)
endif()

add_executable(InstrTest src/testInstr.c)
target_link_libraries(InstrTest z80_lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions)

add_library(z80_lib
    src/mmu.c
    src/cpu.c
    src/cartridge.c
    ${PROJECT_SOURCE_DIR}/src/Instructions/extended.c
    ${PROJECT_SOURCE_DIR}/src/Instructions/bitmanip.c
    ${PROJECT_SOURCE_DIR}/src/Instructions/arithmetic.c
    ${PROJECT_SOURCE_DIR}/src/Instructions/movement.c
    ${PROJECT_SOURCE_DIR}/src/Instructions/extended.c
    src/loop.c
)


add_subdirectory(tests)
