//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_QUADTREE_H
#define QUADKDBENCH_QUADTREE_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class QuadTree {
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

    vector<Point> query(Area queryRectangle);

    bool contains(Point point);

    bool isEmpty();

    void add(Point point);
};


#endif //QUADKDBENCH_QUADTREE_H
