//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_MYKDTREE_H
#define QUADKDBENCH_MYKDTREE_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class MyKDTree {

    struct CompareMKDTree {
        const Point &queryPoint;

        explicit CompareMKDTree(const Point &p) : queryPoint(p) {}

        bool operator()(MyKDTree *a, MyKDTree *b) const {
            return sqDistanceFrom(a->area, queryPoint) > sqDistanceFrom(b->area, queryPoint);
        }
    };

private:
    Area area{};
    int level;
    vector<Point> points;
    MyKDTree *leftChild{}, *rightChild{};

    void deleteTree(MyKDTree *node);

    void setVerticalChildren(int level);

    void setHorizontalChildren(int level);

    void appendPoint(Point &point, int level);

    void kNearestNeighborsHelper(MyKDTree *node, int k,
                                 priority_queue<MyKDTree *, std::vector<MyKDTree *>, CompareMKDTree> &queue,
                                 std::vector<Point> &result);

public:

    MyKDTree(vector<Point> &points, Area &area, int level);

    ~MyKDTree();

    bool contains(Point p);

    list<Point> query(Area &queryArea);

    void buildTree();

    void buildTree(int level);

    int getHeight();

    bool isLeaf();

    bool isEmpty();

    void add(Point &point);

    vector<Point> kNearestNeighbors(Point &point, int k);
};


#endif //QUADKDBENCH_MYKDTREE_H
