cmake_minimum_required (VERSION 2.8.11)
project (CS687)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -D _USE_MULTITHREADING -O3 ")

file(GLOB_RECURSE SRCS ./agents *.cpp)
file(GLOB_RECURSE SRCS ./bases *.cpp)
file(GLOB_RECURSE SRCS ./environments *.cpp)
file(GLOB_RECURSE SRCS ./experimental *.cpp)
file(GLOB_RECURSE SRCS ./other *.cpp)
file(GLOB_RECURSE SRCS ./policies *.cpp)

include_directories(../include)
include_directories(../header)

add_executable (RLCPP ${SRCS} )

