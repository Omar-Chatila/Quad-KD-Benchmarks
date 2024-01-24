cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Add source files
set(SOURCES
    BenchSpace.cpp
    ../MyKDTree.cpp
    ../KDTreeEfficient.cpp
    ../QuadTree.cpp
    malloc_count.c
)

# Set compiler flags
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Specify the executable name and source files
add_executable(main ${SOURCES})
