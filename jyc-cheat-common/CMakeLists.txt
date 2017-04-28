cmake_minimum_required (VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#include_directories(./)
file(GLOB SOURCES
    ./*.hpp
    ./*.cpp
)
add_library(jyc_cheat_common ${SOURCES})




