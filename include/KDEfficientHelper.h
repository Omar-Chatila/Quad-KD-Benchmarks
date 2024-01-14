//
// Created by omarc on 13/01/2024.
//

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "KDTreeEfficient.h"
#include "Util.h"

using namespace std;

inline KDTreeEfficient buildTreeFromFile(int pointNumber) {
    std::ifstream inputFile(R"(C:\Users\omarc\CLionProjects\QuadKDBench\random_points.txt)");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file\n";
    }

    int size = pointNumber - 1;
    Point pointArray[pointNumber];
    std::vector<Point> points = getRandomPoints(pointNumber);

    int i = 0;
    for (auto point: points) {
        pointArray[i++] = point;
    }

    Area area{0, 10000, 0, 10000};
    int start = 0;
    KDTreeEfficient kdTreeEfficient(pointArray, 0, area, start, size);
    kdTreeEfficient.buildTree();

    return kdTreeEfficient;
}

inline list<Point> testQuery(KDTreeEfficient &kdTreeEfficient) {
    Area queryArea(234, 7000, 2000, 9000);
    struct timespec start{}, now{};
    clock_gettime(CLOCK_MONOTONIC, &start);
    list<Point> result = kdTreeEfficient.query(queryArea);
    clock_gettime(CLOCK_MONOTONIC, &now);
    printf("Elapsed: %lf seconds\n", (now.tv_sec - start.tv_sec) + 1e-9 * (now.tv_nsec - start.tv_nsec));
    return result;
}


