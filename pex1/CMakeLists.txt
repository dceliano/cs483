cmake_minimum_required(VERSION 3.6)
project(pex1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES list.c list.h listtest.c shell.c)
add_executable(pex1 ${SOURCE_FILES})