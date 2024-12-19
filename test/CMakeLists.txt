cmake_minimum_required(VERSION 3.25)
set(PROJECT_NAME "test")

include_directories("include")
set(SOURCES main.cpp src/*.cpp include/tpmacros.h)

add_compile_options()

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} main.cpp)
