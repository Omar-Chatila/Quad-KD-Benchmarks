//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_KDTREEEFFICIENT_H
#define QUADKDBENCH_KDTREEEFFICIENT_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class KDTreeEfficient {

private:

    Point *points;
    Area area;
    int from, to;
    KDTreeEfficient *leftChild;
    KDTreeEfficient *rightChild;
    double xMedian, yMedian;

    void buildTree(int level);

    void setVerticalChildren(int level);

    void setHorizontalChildren(int level);

public:
    KDTreeEfficient(Point *points, int level, Area area, int from, int to);

    bool contains(Point p);

    vector<Point> query(Area queryArea);

    void buildTree();

    int getHeight();

    bool isLeaf();
};


#endif //QUADKDBENCH_KDTREEEFFICIENT_H
