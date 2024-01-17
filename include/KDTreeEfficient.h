//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_KDTREEEFFICIENT_H
#define QUADKDBENCH_KDTREEEFFICIENT_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class KDTreeEfficient {

    friend std::ostream &operator<<(std::ostream &os, const KDTreeEfficient &kdTreeEfficient) {
        os << std::fixed << std::setprecision(1);

        return os << "A:" << kdTreeEfficient.area << "f:" << kdTreeEfficient.from << "t:" << kdTreeEfficient.to
                  << "xMed" << std::max(kdTreeEfficient.xMedian, 0.0) << "yMed"
                  << std::max(kdTreeEfficient.yMedian, 0.0) << "\n";
    }

private:

    Point *points;
    Area area{};
    int from, to;
    KDTreeEfficient *leftChild{};
    KDTreeEfficient *rightChild{};
    double xMedian, yMedian;

    void buildTree(int level);

    void setVerticalChildren(int level);

    void setHorizontalChildren(int level);

public:
    KDTreeEfficient(Point *points, int level, Area &area, int from, int to);

    ~KDTreeEfficient();

    bool contains(Point p);

    list<Point> query(Area queryArea);

    void buildTree();

    int getHeight();

    bool isLeaf();

    Point *getPoints();

    KDTreeEfficient *getLeftChild();

    KDTreeEfficient *getRightChild();
};


#endif //QUADKDBENCH_KDTREEEFFICIENT_H
