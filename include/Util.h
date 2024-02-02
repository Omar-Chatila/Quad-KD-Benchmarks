/**
 * @author Omar Chatila
 * @file Util.h
 * @brief Various utility functions and structs
 */

#pragma once

#include "vector"
#include "cstdlib"
#include "iostream"
#include <fstream>
#include <sstream>
#include <random>
#include <set>
#include <unordered_set>
#include "algorithm"


using namespace std;

enum Quadrant {
    NORTH_WEST, NORTH_EAST,
    SOUTH_WEST, SOUTH_EAST
};

/**
 * @brief A struct representing a 2D point with x and y coordinates.
 */
struct Point {
    double x; /**< The x-coordinate of the point. */
    double y; /**< The y-coordinate of the point. */

    /**
     * @brief Overloaded << operator to stream the Point information.
     * @param os The output stream.
     * @param point The Point instance to be streamed.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Point &point) {
        return os << "[" << (point.x) << ":" << (point.y) << "]";
    }

    /**
     * @brief Overloaded == operator to check if two points are equal.
     * @param other The other Point to compare with.
     * @return True if the x and y coordinates of the points are equal; false otherwise.
     */
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    /**
     * @brief Hash specialization for the Point struct.
     */
    template<>
    struct hash<Point> {
        /**
         * @brief Operator to generate a unique hash for a Point.
         * @param p The Point for which the hash is generated.
         * @return The hash value.
         */
        size_t operator()(const Point &p) const {
            // Combine the hashes of x and y to generate a unique hash for the point
            return hash<double>()(p.x) ^ (hash<double>()(p.y) << 1);
        }
    };
}

/**
 * Area struct for Quadtree squares and KD-Tree rectangles
 */
struct Area {
    friend std::ostream &operator<<(std::ostream &os, const Area &area) {
        return os << "[" << (area.xMin) << ":" << (area.xMax) << "] : ["
                  << (area.yMin) << ":" << (area.yMax) << ']';
    }

    /**
     * @brief Overloaded == operator to check if two Areas are equal.
     * @param other The other Point to compare with.
     * @return True if areas are equal; false otherwise.
     */
    bool operator==(const Area &other) const {
        return xMin == other.xMin && yMin == other.yMin
               && xMax == other.xMax && yMax == other.yMax;
    }

    double xMin, xMax, yMin, yMax;
};

/**
 * @brief splits area into 4 quadrants and returns them
 * @param area Area to be split
 * @param xMid mid x-coordinate
 * @param yMid mid y-coordinate
 * @return array of the 4 quadrants
 */
inline Area *splitArea(Area &area, double xMid, double yMid) {
    Area *areas = (Area *) std::malloc(sizeof(Area) << 2);
    areas[NORTH_EAST] = Area{xMid, area.xMax, yMid, area.yMax};
    areas[NORTH_WEST] = Area{area.xMin, xMid, yMid, area.yMax};
    areas[SOUTH_WEST] = Area{area.xMin, xMid, area.yMin, yMid};
    areas[SOUTH_EAST] = Area{xMid, area.xMax, area.yMin, yMid};
    return areas;
}

/**
 * @brief Checks if two areas intersect each other
 *
 * Note: also returns true if borders overlap but interception area is 0
 *
 * @param first  area
 * @param other  area
 * @return true if they intersect each other, otherwise false
 */
inline bool intersects(Area &first, Area &other) {
    return first.xMin <= other.xMax && first.xMax >= other.xMin && first.yMax >= other.yMin && first.yMin <= other.yMax;
}

/**
 * @brief Checks if first area intersects the second
 * @param container Area possibly containing contained
 * @param contained Area possibly contained by container
 * @return True if container contains contained, false otherwise
 */
inline bool containsArea(Area &container, Area &contained) {
    return container.xMin <= contained.xMin && container.xMax >= contained.xMax && container.yMin <= contained.yMin &&
           container.yMax >= contained.yMax;
}

/**
 * @brief Checks if area contains point
 * @param area
 * @param point
 * @return true if area contains point, otherwise false
 */
inline bool containsPoint(Area &area, Point &point) {
    return (point.x >= area.xMin && point.y >= area.yMin && point.x <= area.xMax && point.y <= area.yMax);
}

/**
 * @brief returns median of a points array based on coordinate within specified bounds
 * returns median and rearranges array such that points larger than median right
 * and points lower are left of median index
 * @param points array of points objects
 * @param x true if median by x-coordinates, otherwise y-coordinates
 * @param left index: left bound of array
 * @param right index: right bound of array
 * @return median of specified coordinate
 */
inline double median(Point *points, bool x, int left, int right) {
    int size = right - left;
    int pos = left + size / 2;

    std::nth_element(points + left, points + pos, points + right, [&x](const Point &a, const Point &b) {
        return x ? a.x < b.x : a.y < b.y;
    });

    return x ? points[pos].x : points[pos].y;
}

/**
 * @brief splits vector in half and returns two halves
 * @param originalVector vector to be split
 * @return vector containing the two split vectors
 */
inline vector<vector<Point>> splitVector(vector<Point> &originalVector) {
    long long midIndex = static_cast<long long>(originalVector.size()) / 2;

    vector<Point> subVector1(originalVector.begin(), originalVector.begin() + midIndex);
    vector<Point> subVector2(originalVector.begin() + midIndex, originalVector.end());

    return {subVector1, subVector2};
}

/**
 * @brief Returns value of Point in the middle of the list.
 * If size is even, average of both mid values is returned
 * @param list pointlist
 * @param x true if x-coordinate is required, otherwise false
 * @return value of middle point
 */
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

/**
 * @brief Creates vector with uniformly distributed random points and proportionally large bounds
 * @param pointNumber Number of points to be created
 * @return vector with random points
 */
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

/**
 * @brief Creates vector with uniformly distributed random points in an area of constant size
 * the higher the point number the higher the density
 * @param pointNumber number of points
 * @return vector with random points
 */
inline vector<Point> getDensePoints(int pointNumber) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, 1000);

    vector<Point> points;
    points.reserve(pointNumber);
    std::unordered_set<Point> generatedPoints;
    for (int i = 0; i < pointNumber;) {
        double x = dis(gen);
        double y = dis(gen);
        Point newPoint{x, y};
        if (generatedPoints.find({x, y}) == generatedPoints.end()) {
            points.push_back(newPoint);
            generatedPoints.insert({x, y});
            i++;
        }
    }
    return points;
}

/**
 * @brief Creates array with uniformly distributed random points in an area of constant size
 * the higher the point number the higher the density
 * @param pointNumber number of points
 * @return array with random points
 */
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

/**
 * @brief Creates array with uniformly distributed random points in an area of constant size
 *
 * Area with bounds [0:1000] x [0:1000]. The higher the point number the higher the density
 *
 * @param pointNumber number of points
 * @return array with random points
 */
inline Point *getDensePointsArray(int pointNumber) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, 1000);

    auto *points = (Point *) malloc(pointNumber * sizeof(Point));
    std::unordered_set<Point> generatedPoints;

    for (int i = 0; i < pointNumber;) {
        double x = dis(gen);
        double y = dis(gen);
        Point newPoint{x, y};
        if (generatedPoints.find({x, y}) == generatedPoints.end()) {
            points[i] = newPoint;
            generatedPoints.insert({x, y});
            i++;
        }
    }
    return points;
}

/**
 * @brief Creates random point within [0:bounds] x [0:bounds]
 * @param bounds highest x/y coordinate of point
 * @return point
 */
inline Point getRandomPoint(int bounds) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, bounds);
    return {dis(gen), dis(gen)};
}

/**
 * Calculates distance between area and point
 * @param area for distance calculation
 * @param point for distance calculation
 * @return distance between area an point
 */
inline double sqDistanceFrom(const Area &area, const Point &point) {
    double dx = max(max(area.xMin - point.x, 0.0), point.x - area.xMax);
    double dy = max(max(area.yMin - point.y, 0.0), point.y - area.yMax);
    return dx * dx + dy * dy;
}

/**
 * Calculates squared distance between two points
 * @param p1 first point
 * @param p2 second point
 * @return squared distance
 */
inline double pointDistance(const Point &p1, const Point &p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}