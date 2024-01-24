cmake_minimum_required(VERSION 3.27)
project(QuadKDBench)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Add the path to malloc_count.o and link with -ldl
add_library(malloc_count OBJECT src/spacer/malloc_count.c
        src/spacer/BenchSpace.cpp
        benchmark/include/benchmark/benchmark.h
        benchmark/include/benchmark/export.h
        include/PointRegionQuadTree.h
)
target_compile_options(malloc_count PRIVATE -ldl)

add_executable(QuadKDBench src/main.cpp
        src/QuadTree.cpp
        include/QuadTree.h
        include/Util.h
        src/KDTreeEfficient.cpp
        include/KDTreeEfficient.h
        src/MyKDTree.cpp
        include/MyKDTree.h
        include/TreeHelper.h
        $<TARGET_OBJECTS:malloc_count>
        include/PointRegionQuadTree.h
        src/PointRegionQuadTree.cpp
)
# Include directories
target_include_directories(QuadKDBench PRIVATE include)

# Link with -ldl
target_link_libraries(QuadKDBench PRIVATE dl)
#target_link_libraries(QuadKDBench benchmark::benchmark)

#add_subdirectory(benchmark)
