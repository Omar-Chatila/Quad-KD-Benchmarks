//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_KDTREEEFFICIENT_H
#define QUADKDBENCH_KDTREEEFFICIENT_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class KDTreeEfficient {

    struct CompareKDETree {
        const Point &queryPoint;

        explicit CompareKDETree(const Point &p) : queryPoint(p) {}

        bool operator()(KDTreeEfficient *a, KDTreeEfficient *b) const {
            return sqDistanceFrom(a->area, queryPoint) > sqDistanceFrom(b->area, queryPoint);
        }
    };

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

    void kNearestNeighborsHelper(KDTreeEfficient *node, int k,
                                 priority_queue<KDTreeEfficient *, std::vector<KDTreeEfficient *>, CompareKDETree> &queue,
                                 std::vector<Point> &result);

public:
    KDTreeEfficient(Point *points, int level, Area &area, int from, int to);

    ~KDTreeEfficient();

    bool contains(Point p);

    list<Point> query(Area queryArea);

    void buildTree();

    int getHeight();

    bool isLeaf() const;

    Point *getPoints();

    KDTreeEfficient *getLeftChild();

    KDTreeEfficient *getRightChild();

    vector<Point> kNearestNeighbors(Point &point, int k);
};


#endif //QUADKDBENCH_KDTREEEFFICIENT_H
