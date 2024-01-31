/**
 * @author Omar Chatila
 * @file Util.h
 * @brief Various utility functions for creation of trees
 * and naive implementations for benchmarks
 */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "KDTreeEfficient.h"
#include "KDBTreeEfficient.h"
#include "QuadTree.h"
#include "Util.h"
#include "SortKDTree.h"
#include "PointRegionQuadTree.h"
#include "KDBTreeEfficient.h"

using namespace std;

/**
 * @brief Builds an efficient KD-Tree containing pointNumber points
 * @param pointNumber number of random points
 * @return KDTreeEfficient containing random points
 */
inline KDTreeEfficient *buildEKD_Random(int pointNumber) {
    auto *pointArray = (Point *) malloc(pointNumber * sizeof(Point));
    std::vector<Point> points = getRandomPoints(pointNumber);
    int i = 0;
    for (auto point: points) {
        pointArray[i++] = point;
    }
    double bounds = pointNumber;
    Area area{0, bounds, 0, bounds};
    auto *kdTreeEfficient = new KDTreeEfficient(pointArray, area, pointNumber);
    kdTreeEfficient->buildTree();
    return kdTreeEfficient;
}

/**
 * @brief Builds an efficient KDB-Tree containing pointNumber points
 * @param pointNumber number of random points
 * @return KDBTreeEfficient containing random points
 */
inline KDBTreeEfficient *buildKDB_Random(int pointNumber) {
    int size = pointNumber - 1;
    auto *pointArray = (Point *) malloc(pointNumber * sizeof(Point));
    std::vector<Point> points = getRandomPoints(pointNumber);
    int i = 0;
    for (auto point: points) {
        pointArray[i++] = point;
    }
    double bounds = pointNumber;
    Area area{0, bounds, 0, bounds};
    int start = 0;
    int capacity = (int) max(log10(pointNumber), 4.0);
    auto *kdbTreeEfficient = new KDBTreeEfficient(pointArray, 0, area, start, size, capacity);
    kdbTreeEfficient->buildTree();
    return kdbTreeEfficient;
}

/**
 * @brief Builds an efficient Quadtree containing pointNumber points
 * @param pointNumber number of random points
 * @return Quadtree containing random points
 */
inline QuadTree *buildQuadTreeRandom(int pointNumber) {
    std::vector<Point> points = getRandomPoints(pointNumber);
    double bounds = pointNumber;
    Area area{0, bounds, 0, bounds};
    auto *quadTree = new QuadTree(area, points);
    quadTree->buildTree();
    return quadTree;
}

/**
 * @brief Builds an Point-Region-Quadtree containing pointNumber points
 * @param pointNumber number of random points
 * @return Point-Region-Quadtree containing random points
 */
inline PointRegionQuadTree *buildPRQuadTreeRandom(int pointNumber) {
    std::vector<Point> points = getRandomPoints(pointNumber);
    double bounds = pointNumber;
    int capacity = (int) max(log10(pointNumber), 4.0);
    Area area{0, bounds, 0, bounds};
    auto quadTree = new PointRegionQuadTree(area, points, capacity);
    quadTree->buildTree();
    return quadTree;
}

/**
 * @brief Builds a SortKDTree containing pointNumber points
 * @param pointNumber number of random points
 * @return SortKDTree containing random points
 */
inline SortKDTree *buildSortKDTreeRandom(int pointNumber) {
    vector<Point> points = getRandomPoints(pointNumber);
    double bounds = pointNumber;
    Area area{0, bounds, 0, bounds};
    auto myKdTree = new SortKDTree(points, area);
    myKdTree->buildTree();
    return myKdTree;
}

/**
 * @brief Naive range query-algorithm
 * @param points points of interest
 * @param queryArea area containing points
 * @return points of points-vector that are contained by queryArea
 */
inline list<Point> getQueryNaive(vector<Point> &points, Area &queryArea) {
    list<Point> result;
    for (auto point: points) {
        if (containsPoint(queryArea, point)) {
            result.push_back(point);
        }
    }
    return result;
}

/**
 * @brief Runs linear search on pointVector with every element in searchPoints
 * @param searchPoints search Points
 * @param points vector to be searched
 * @return dummy return value
 */
inline bool containsNaive(vector<Point> &searchPoints, vector<Point> &points) {
    list<Point> result;
    bool contains;
    for (auto searchPoint: searchPoints) {
        for (auto point: points) {
            if (point == searchPoint) {
                contains = true;
                break;
            }
        }
    }
    return contains;
}

/**
 * @brief Runs Quadtree contains function on several points
 * @param quadtree Quadtree
 * @param points Points
 */
inline void qtContainsPoint(QuadTree *quadtree, vector<Point> &points) {
    for (auto point: points) {
        quadtree->contains(point);
    }
}

/**
 * @brief Runs Point-Region-Quadtree contains function on several points
 * @param quadtree PR-Quadtree
 * @param points Points
 */
inline void pr_qtContainsPoint(PointRegionQuadTree *quadtree, vector<Point> &points) {
    for (auto point: points) {
        quadtree->contains(point);
    }
}

/**
 * @brief Runs KDTreeEfficient contains function on several points
 * @param kdTreeEfficient PR-Quadtree
 * @param points Points
 */
inline void kdEContainsPoint(KDTreeEfficient *kdTreeEfficient, vector<Point> &points) {
    for (auto point: points) {
        kdTreeEfficient->contains(point);
    }
}

/**
 * @brief Runs KDBTreeEfficient contains function on several points
 * @param kdTreeEfficient PR-Quadtree
 * @param points Points
 */
inline void kdbContainsPoint(KDBTreeEfficient *kdTreeEfficient, vector<Point> &points) {
    for (auto point: points) {
        kdTreeEfficient->contains(point);
    }
}

/**
 * @brief Runs SortKDTree contains function on several points
 * @param tree SKD-Tree
 * @param points points
 */
inline void sKDContainsPoint(SortKDTree *tree, vector<Point> &points) {
    for (auto point: points) {
        tree->contains(point);
    }
}

/**
 * @brief Naive implementation of k-NNS. Sorts vector and returns first k-Elements of sorted list
 * @param queryPoint point of which nearest neighbors should be calculated
 * @param points that should be queried
 * @param k number of neighbors
 * @return list containing k nearest neighbors
 */
inline vector<Point> naive_kNNS(Point &queryPoint, vector<Point> &points, int k) {
    sort(points.begin(), points.end(), [queryPoint](const Point &lhs, const Point &rhs) {
        return pointDistance(lhs, queryPoint) < pointDistance(rhs, queryPoint);
    });
    return {points.begin(), points.begin() + k};
}


