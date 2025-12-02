cmake_minimum_required(VERSION 3.10)

project(Tema2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(program src/main.cpp)

add_custom_target(run
    COMMAND $<TARGET_FILE:program>
    DEPENDS program
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    USES_TERMINAL
)