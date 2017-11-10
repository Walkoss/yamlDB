cmake_minimum_required(VERSION 3.8)
project(yamlDB)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES sources/main.c)
add_executable(yamlDB ${SOURCE_FILES})