cmake_minimum_required(VERSION 3.10)
project(Decoder)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.c")
add_executable(Decoder ${SOURCES})

if(UNIX AND NOT APPLE)
    target_link_libraries(Decoder m)
endif()