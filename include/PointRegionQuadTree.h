//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_PointRegionQuadTree_H
#define QUADKDBENCH_PointRegionQuadTree_H

#include "Util.h"
#include <bits/stdc++.h>

class PointRegionQuadTree {

    struct ComparePointRegionQuadTree {
        const Point &queryPoint;

        explicit ComparePointRegionQuadTree(const Point &p) : queryPoint(p) {}

        bool operator()(PointRegionQuadTree *a, PointRegionQuadTree *b) const {
            return sqDistanceFrom(a->square, queryPoint) > sqDistanceFrom(b->square, queryPoint);
        }
    };

    friend std::ostream &operator<<(std::ostream &os, const PointRegionQuadTree &PointRegionQuadTree) {
        os << std::fixed << std::setprecision(1);
        if (!PointRegionQuadTree.elements.empty())
            return os << "A:" << PointRegionQuadTree.square << "elements:" << PointRegionQuadTree.elements.front() << "\n";
        return os << "A:" << PointRegionQuadTree.square << "\n";
    }

private:
    PointRegionQuadTree *children[4]{};
    Area square{};
    vector<Point> elements;
    int capacity;

    static PointRegionQuadTree *locateQuadrant(double pointX, double pointY, PointRegionQuadTree *current);

    void kNearestNeighborsHelper(PointRegionQuadTree *node, int k,
                                 priority_queue<PointRegionQuadTree *, std::vector<PointRegionQuadTree *>, ComparePointRegionQuadTree> &queue,
                                 std::vector<Point> &result, Point &queryPoint);

public:

    PointRegionQuadTree(Area square, vector<Point> &elements, int capacity);

    bool isNodeLeaf();

    bool isPointLeaf();

    int getHeight();

    void partition();

    void buildTree();

    list<Point> query(Area &queryRectangle);

    bool contains(Point &point);

    bool isEmpty();

    void add(Point &point);

    PointRegionQuadTree *getNorthEast();

    PointRegionQuadTree *getNorthWest();

    PointRegionQuadTree *getSouthWest();

    PointRegionQuadTree *getSouthEast();

    vector<Point> kNearestNeighbors(Point &queryPoint, int k);
};


#endif //QUADKDBENCH_PointRegionQuadTree_H
