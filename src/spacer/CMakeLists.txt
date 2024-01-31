cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Add source files
set(SOURCES
        BenchSpace.cpp
        ../SortKDTree.cpp
        ../KDTreeEfficient.cpp
        ../QuadTree.cpp
        ../PointRegionQuadTree.cpp
        malloc_count.c
)

# Set compiler flags
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Specify the executable name and source files
add_executable(benchspace ${SOURCES})
