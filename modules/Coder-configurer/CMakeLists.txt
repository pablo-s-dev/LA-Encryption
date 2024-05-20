cmake_minimum_required(VERSION 3.10)
project(Coder-configurer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.c")
add_executable(Coder-configurer ${SOURCES})
