//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_MYKDTREE_H
#define QUADKDBENCH_MYKDTREE_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class MyKDTree {

private:
    Area area;
    int level;
    vector<Point> points;
    MyKDTree *leftChild, *rightChild;
    SplitLine verticalSplitLine, horizontalSplitLine;

    void buildTree(int level);

    void setVerticalChildren(int level);

    void setHorizontalChildren(int level);

    void appendPoint(Point point, int level);

public:

    MyKDTree(vector<Point> &points, Area area, int level);

    bool contains(Point p);

    vector<Point> query(Area queryArea);

    void buildTree();

    int getHeight();

    bool isLeaf();

    void setSplitLines();

    bool isEmpty();

    void add(Point point);
};


#endif //QUADKDBENCH_MYKDTREE_H