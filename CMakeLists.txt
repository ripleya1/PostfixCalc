cmake_minimum_required(VERSION 3.19 FATAL_ERROR) # check cmake version
project(postfixCalc
    LANGUAGES C # use C
)

add_library(
    calcLib
    STATIC
    postfixCalc.h
    queue.c
    queue.h
    stack.c
    stack.h
)
add_executable(calc postfixCalc.c)

target_link_libraries(calc PUBLIC calcLib m) # link internal files and math.h library
