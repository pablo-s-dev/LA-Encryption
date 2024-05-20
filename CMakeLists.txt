cmake_minimum_required(VERSION 3.10)

project(La-encryption)

add_subdirectory(modules/Coder)
add_subdirectory(modules/Decoder)
add_subdirectory(modules/Coder-configurer)
add_subdirectory(modules/HelloWorld)

add_executable(La-encryption main.c)
