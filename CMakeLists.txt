cmake_minimum_required(VERSION 3.3)
project(pasteit)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/Users/denis/Desktop/Paste/build")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")

set(SOURCE_FILES
        inc/Paste.h
        main.cpp src/Paste.cpp src/Application.cpp inc/Application.h)

add_executable(pasteit ${SOURCE_FILES})