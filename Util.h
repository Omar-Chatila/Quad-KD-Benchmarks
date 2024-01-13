//
// Created by omarc on 12/01/2024.
//

#pragma once

#include "vector"
#include "cstdlib"
#include "iostream"

using namespace std;

enum Quadrant {
    NORTH_WEST, NORTH_EAST,
    SOUTH_WEST, SOUTH_EAST
};

struct Point {
    double x, y;

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

struct SplitLine {
    double fromX, fromY, toX, toY;
};

inline Area *splitArea(Area area) {
    double xMid = (area.xMin + area.xMax) / 2.0;
    double yMid = (area.yMin + area.yMax) / 2.0;
    Area *areas = (Area *) std::malloc(sizeof(Area) << 2);
    areas[NORTH_EAST] = Area(xMid, area.xMax, yMid, area.yMax);
    areas[NORTH_WEST] = Area(area.xMin, xMid, yMid, area.yMax);
    areas[SOUTH_WEST] = Area(area.xMin, xMid, area.yMin, yMid);
    areas[SOUTH_EAST] = Area(xMid, area.xMax, area.yMin, yMid);
    return areas;
}

inline bool intersects(Area &first, Area &other) {
    return first.xMin < other.xMax && first.xMax > other.xMin && first.yMax > other.yMin && first.yMin < other.yMax;
}

inline bool containsArea(Area &first, Area &other) {
    return first.xMin <= other.xMin && first.xMax >= other.xMax && first.yMin <= other.yMin && first.yMax >= other.yMax;
}

inline bool containsPoint(Area &area, Point &point) {
    return (point.x >= area.xMin && point.y >= area.yMin && point.x <= area.xMax && point.y <= area.yMax);
}

inline void swap(Point *points, int first, int second) {
    Point temp = points[first];
    points[first] = points[second];
    points[second] = temp;
}

inline int partition(Point *points, int left, int right, bool x) {
    double pivot = x ? points[left].x : points[left].y;
    int position = left;
    for (int i = left + 1; i <= right; i++) {
        if ((x ? points[i].x : points[i].y) <= pivot) {
            position++;
            swap(points, i, position);
        }
    }
    swap(points, left, position);
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

inline double median(Point *points, bool x, int left, int right) {
    int pos = (left + right) / 2;
    return quickSelect(points, pos, left, right, x);
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