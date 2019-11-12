cmake_minimum_required(VERSION 3.15)
project(_PROJECT_NAME_ C)

set(CMAKE_C_STANDARD 11)

#MATH(EXPR stack_size "100*1024*1024*1024*1024")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,99999999999999")

add_executable(_PROJECT_NAME_ _CODE_NAME.c_)