cmake_minimum_required(VERSION 3.6)
project(pex2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c lrustack.c)
add_executable(pex2 ${SOURCE_FILES})