cmake_minimum_required(VERSION 3.27)
project(QuadKDBench)

set(CMAKE_CXX_STANDARD 23)

add_executable(QuadKDBench main.cpp
        QuadTree.cpp
        QuadTree.h
        Util.h
        KDTreeEfficient.cpp
        KDTreeEfficient.h
        MyKDTree.cpp
        MyKDTree.h
        KDEfficientHelper.cpp
        KDEfficientHelper.h
)
