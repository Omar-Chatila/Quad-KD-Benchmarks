cmake_minimum_required(VERSION 3.27)
project(QuadKDBench)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wl,--stack,1000000000")

add_executable(QuadKDBench src/main.cpp
        src/QuadTree.cpp
        include/QuadTree.h
        include/Util.h
        src/KDTreeEfficient.cpp
        include/KDTreeEfficient.h
        src/MyKDTree.cpp
        include/MyKDTree.h
        include/TreeHelper.h
)

#target_link_libraries(QuadKDBench benchmark::benchmark)

#add_subdirectory(benchmark)
