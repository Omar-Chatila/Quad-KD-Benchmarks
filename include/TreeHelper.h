//
// Created by omarc on 13/01/2024.
//

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "KDTreeEfficient.h"
#include "QuadTree.h"
#include "Util.h"
#include "MyKDTree.h"

using namespace std;

inline KDTreeEfficient buildEKDTreeFromFile(int pointNumber) {
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

inline QuadTree buildQuadTreeFromFile(int pointNumber) {
    std::vector<Point> points = getRandomPoints(pointNumber);
    Area area{0, 10000, 0, 10000};
    QuadTree quadTree(area, points);
    quadTree.buildTree();
    return quadTree;
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

inline MyKDTree buildMyKDFromFile(int pointNumber) {
    vector<Point> points = getRandomPoints(pointNumber);
    Area area{0, 10000, 0, 10000};
    MyKDTree myKdTree(points, area, 0);
    myKdTree.buildTree();
    return myKdTree;
}

inline list<Point> getQueryNaive(vector<Point> &points, Area &queryArea) {
    list<Point> result;
    for (auto point: points) {
        if (containsPoint(queryArea, point)) {
            result.push_back(point);
        }
    }
    return result;
}


