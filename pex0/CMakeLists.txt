cmake_minimum_required(VERSION 3.6)
project(Documents)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES pex0.c pex0.h)
add_executable(Documents ${SOURCE_FILES})