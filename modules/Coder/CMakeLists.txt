cmake_minimum_required(VERSION 3.10)
project(Coder)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.c")
add_executable(Coder ${SOURCES})

if(UNIX AND NOT APPLE)
    target_link_libraries(Coder m)
endif()