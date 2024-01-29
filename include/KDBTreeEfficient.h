//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_KDBTreeEfficient_H
#define QUADKDBENCH_KDBTreeEfficient_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class KDBTreeEfficient {

    struct CompareKDBTree {
        const Point &queryPoint;

        explicit CompareKDBTree(const Point &p) : queryPoint(p) {}

        bool operator()(KDBTreeEfficient *a, KDBTreeEfficient *b) const {
            return sqDistanceFrom(a->area, queryPoint) > sqDistanceFrom(b->area, queryPoint);
        }
    };

    friend std::ostream &operator<<(std::ostream &os, const KDBTreeEfficient &KDBTreeEfficient) {
        os << std::fixed << std::setprecision(1);

        return os << "A:" << KDBTreeEfficient.area << "f:" << KDBTreeEfficient.from << "t:" << KDBTreeEfficient.to
                  << "xMed" << std::max(KDBTreeEfficient.xMedian, 0.0) << "yMed"
                  << std::max(KDBTreeEfficient.yMedian, 0.0) << "\n";
    }

private:
    Point *points;
    Area area{};
    int from, to;
    int capacity;
    KDBTreeEfficient *leftChild{};
    KDBTreeEfficient *rightChild{};
    double xMedian, yMedian;

    void buildTree(int level);

    void setVerticalChildren(int level);

    void setHorizontalChildren(int level);

    void kNearestNeighborsHelper(KDBTreeEfficient *node, int k,
                                 priority_queue<KDBTreeEfficient *, std::vector<KDBTreeEfficient *>, CompareKDBTree> &queue,
                                 std::vector<Point> &result, Point &point);

public:
    KDBTreeEfficient(Point *points, int level, Area &area, int from, int to, int capacity);

    ~KDBTreeEfficient();

    bool contains(Point p);

    list<Point> query(Area queryArea);

    void buildTree();

    int getHeight();

    bool isLeaf() const;

    Point *getPoints();

    KDBTreeEfficient *getLeftChild();

    KDBTreeEfficient *getRightChild();

    vector<Point> kNearestNeighbors(Point &point, int k);

};


#endif //QUADKDBENCH_KDBTreeEfficient_H
