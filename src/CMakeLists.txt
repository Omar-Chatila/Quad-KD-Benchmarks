cmake_minimum_required(VERSION 3.16)
project(YourBenchmarkProjectName)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O3")

# Add your benchmark source files
set(BENCHMARK_SOURCES
        Benchmark.cpp
        QuadTree.cpp
        SortKDTree.cpp
        KDTreeEfficient.cpp
        PointRegionQuadTree.cpp
        KDBTreeEfficient.cpp
)

# Add benchmark dependencies (assuming benchmark library is in benchmark/include and benchmark/build/src)
include_directories(SYSTEM benchmark/include)
link_directories(benchmark/build/src)

# Create an executable for the benchmark
add_executable(mybenchmark ${BENCHMARK_SOURCES})

# Link the benchmark library and pthread
target_link_libraries(mybenchmark benchmark pthread)