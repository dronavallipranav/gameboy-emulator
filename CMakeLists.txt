cmake_minimum_required(VERSION 3.10)

project(z80_emulator)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

#Link openGL library based on OS
add_executable(SDL2Test src/main.c)
if(APPLE)
    target_link_libraries(SDL2Test ${SDL2_LIBRARIES} "-framework OpenGL")
else()
    target_link_libraries(SDL2Test ${SDL2_LIBRARIES} GL)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/instructions)

add_library(z80_lib
    src/cpu.c
    src/cartridge.c
    ${PROJECT_SOURCE_DIR}/src/Instructions/arithmetic.c
    ${PROJECT_SOURCE_DIR}/src/Instructions/movement.c
    ${PROJECT_SOURCE_DIR}/src/Instructions/extended.c
)


add_subdirectory(tests)
