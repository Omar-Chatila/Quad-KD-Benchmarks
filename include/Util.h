//
// Created by omarc on 12/01/2024.
//

#pragma once

#include "vector"
#include "cstdlib"
#include "iostream"
#include <fstream>
#include <sstream>
#include <random>
#include "algorithm"


using namespace std;

enum Quadrant {
    NORTH_WEST, NORTH_EAST,
    SOUTH_WEST, SOUTH_EAST
};

struct Point {
    double x, y;

    friend std::ostream &operator<<(std::ostream &os, const Point &point) {
        return os << "[" << (point.x) << ":" << (point.y) << "]";
    }

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

struct Area {
    friend std::ostream &operator<<(std::ostream &os, const Area &area) {
        return os << "[" << (area.xMin) << ":" << (area.xMax) << "] : ["
                  << (area.yMin) << ":" << (area.yMax) << ']';
    }

    double xMin, xMax, yMin, yMax;
};

inline Area *splitArea(Area area, double xMid, double yMid) {
    Area *areas = (Area *) std::malloc(sizeof(Area) << 2);
    areas[NORTH_EAST] = Area{xMid, area.xMax, yMid, area.yMax};
    areas[NORTH_WEST] = Area{area.xMin, xMid, yMid, area.yMax};
    areas[SOUTH_WEST] = Area{area.xMin, xMid, area.yMin, yMid};
    areas[SOUTH_EAST] = Area{xMid, area.xMax, area.yMin, yMid};
    return areas;
}

inline bool intersects(Area &first, Area &other) {
    return first.xMin <= other.xMax && first.xMax >= other.xMin && first.yMax >= other.yMin && first.yMin <= other.yMax;
}

inline bool containsArea(Area &first, Area &other) {
    return first.xMin <= other.xMin && first.xMax >= other.xMax && first.yMin <= other.yMin && first.yMax >= other.yMax;
}

inline bool containsPoint(Area &area, Point &point) {
    return (point.x >= area.xMin && point.y >= area.yMin && point.x <= area.xMax && point.y <= area.yMax);
}

inline int getRandom(int low, int high) {
    return rand() % (high - low) + low;
}

inline double medianOfThree(double a, double b, double c) {
    return max(min(a, b), min(max(a, b), c));
}


inline int partition(Point *points, int left, int right, bool x) {
    int randIndex1 = getRandom(left, right);
    int randIndex2 = getRandom(left, right);
    int randIndex3 = getRandom(left, right);

    // Find the median of three values at the randomly selected indices
    double pivot = medianOfThree(x ? points[randIndex1].x : points[randIndex1].y,
                                 x ? points[randIndex2].x : points[randIndex2].y,
                                 x ? points[randIndex3].x : points[randIndex3].y);

    int position = left;
    for (int i = left + 1; i <= right; i++) {
        if ((x ? points[i].x : points[i].y) <= pivot) {
            position++;
            swap(points[i], points[position]);
        }
    }
    swap(points[left], points[position]);
    return position;
}

inline double quickSelect(Point *points, int pos, int left, int right, bool x) {
    if (left == right && left == pos) {
        return x ? points[left].x : points[left].y;
    }
    int posRes = partition(points, left, right, x);
    if (posRes == pos) {
        return x ? points[posRes].x : points[posRes].y;
    } else if (posRes < pos) {
        return quickSelect(points, pos, posRes + 1, right, x);
    } else {
        return quickSelect(points, pos, left, posRes - 1, x);
    }
}

// Implementation of QuickSelect
inline double kthSmallest(Point *a, int left, int right, int k, bool x) {

    while (left <= right) {

        // Partition a[left..right] around a pivot
        // and find the position of the pivot
        int pivotIndex = partition(a, left, right, x);

        // If pivot itself is the k-th smallest element
        if (pivotIndex == k - 1)
            return x ? a[pivotIndex].x : a[pivotIndex].y;

            // If there are more than k-1 elements on
            // left of pivot, then k-th smallest must be
            // on left side.
        else if (pivotIndex > k - 1)
            right = pivotIndex - 1;

            // Else k-th smallest is on right side.
        else
            left = pivotIndex + 1;
    }
    return -1;
}

inline double median(Point *points, bool x, int left, int right) {
    int size = right - left;
    int pos = left + size / 2;

    std::nth_element(points + left, points + pos, points + right, [&x](const Point &a, const Point &b) {
        return x ? a.x < b.x : a.y < b.y;
    });

    return x ? points[pos].x : points[pos].y;
}


inline vector<vector<Point>> splitVector(vector<Point> &originalVector) {
    long long midIndex = static_cast<long long>(originalVector.size()) / 2;

    vector<Point> subVector1(originalVector.begin(), originalVector.begin() + midIndex);
    vector<Point> subVector2(originalVector.begin() + midIndex, originalVector.end());

    return {subVector1, subVector2};
}

inline double getMedian(vector<Point> &list, bool x) {
    if (x) {
        if (list.size() > 1) {
            return (list.size() % 2 == 0) ?
                   (list.at((list.size() / 2)).x + list.at((list.size() / 2) - 1).x) / 2.0 :
                   list.at(list.size() / 2).x;
        }
        return list.at(0).x;
    } else {
        if (list.size() > 1) {
            return (list.size() % 2 == 0) ?
                   (list.at((list.size() / 2)).y + list.at((list.size() / 2) - 1).y) / 2.0 :
                   list.at(list.size() / 2).y;
        }
        return list.at(0).y;
    }
}

inline vector<Point> getRandomPoints(int pointNumber) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, pointNumber);

    vector<Point> points;
    points.reserve(pointNumber);
    for (int i = 0; i < pointNumber; i++) {
        double x = dis(gen);
        double y = dis(gen);
        points.push_back(Point{x, y});
    }
    return points;
}

inline Point *getRandomPointsArray(int pointNumber) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, pointNumber);

    auto *points = (Point *) malloc(pointNumber * sizeof(Point));

    for (int i = 0; i < pointNumber; i++) {
        double x = dis(gen);
        double y = dis(gen);
        Point newPoint{x, y};
        points[i] = newPoint;
    }
    return points;
}

inline Point getRandomPoint(int bounds) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, bounds);
    return {dis(gen), dis(gen)};
}

inline double sqDistanceFrom(const Area &area, const Point &point) {
    double dx = max(max(area.xMin - point.x, 0.0), point.x - area.xMax);
    double dy = max(max(area.yMin - point.y, 0.0), point.y - area.yMax);
    return dx * dx + dy * dy;
}

inline double pointDistance(const Point &p1, const Point &p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}