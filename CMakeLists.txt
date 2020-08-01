cmake_minimum_required(VERSION 3.16)
project(matrix-completion C)

set(CMAKE_C_STANDARD 99)

add_executable(matrix-completion main.c air_data.c air_data.h linear_regression.c linear_regression.h)

configure_file(${PROJECT_SOURCE_DIR}/air.csv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
