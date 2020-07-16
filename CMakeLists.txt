cmake_minimum_required(VERSION 3.16)
project(sgd C)

set(CMAKE_C_STANDARD 99)

add_executable(sgd main.c exam_readfile.c)

configure_file(${PROJECT_SOURCE_DIR}/air.csv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
