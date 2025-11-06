cmake_minimum_required(VERSION 3.10)

project(TemaSunet CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(
    program 
    src/main.cpp 
    src/SoundBuffer.cpp
)

add_custom_target(run
    COMMAND $<TARGET_FILE:program>
    
    DEPENDS program
)
