//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_QUADTREE_H
#define QUADKDBENCH_QUADTREE_H

#include "Util.h"
#include <bits/stdc++.h>

class QuadTree {

    struct CompareQuadTree {
        const Point &queryPoint;

        explicit CompareQuadTree(const Point &p) : queryPoint(p) {}

        bool operator()(QuadTree *a, QuadTree *b) const {
            return sqDistanceFrom(a->square, queryPoint) > sqDistanceFrom(b->square, queryPoint);
        }
    };

    friend std::ostream &operator<<(std::ostream &os, const QuadTree &quadTree) {
        os << std::fixed << std::setprecision(1);
        if (!quadTree.elements.empty())
            return os << "A:" << quadTree.square << "elements:" << quadTree.elements.front() << "\n";
        return os << "A:" << quadTree.square << "\n";
    }

private:
    QuadTree *children[4]{};
    Area square{};
    vector<Point> elements;

    static QuadTree *locateQuadrant(double pointX, double pointY, QuadTree *current);

    void kNearestNeighborsHelper(QuadTree *node, int k,
                                 priority_queue<QuadTree *, std::vector<QuadTree *>, CompareQuadTree> &queue,
                                 std::vector<Point> &result);

public:

    QuadTree(Area square, vector<Point> &elements);

    bool isNodeLeaf();

    bool isPointLeaf();

    int getHeight();

    void partition();

    void buildTree();

    list<Point> query(Area &queryRectangle);

    bool contains(Point &point);

    bool isEmpty();

    void add(Point &point);

    QuadTree *getNorthEast();

    QuadTree *getNorthWest();

    QuadTree *getSouthWest();

    QuadTree *getSouthEast();

    vector<Point> kNearestNeighbors(Point &queryPoint, int k);
};


#endif //QUADKDBENCH_QUADTREE_H
