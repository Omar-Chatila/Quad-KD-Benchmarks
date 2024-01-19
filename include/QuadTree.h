//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_QUADTREE_H
#define QUADKDBENCH_QUADTREE_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class QuadTree {

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

};


#endif //QUADKDBENCH_QUADTREE_H
