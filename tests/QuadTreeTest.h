//
// Created by omarc on 31/01/2024.
//

#ifndef QUADKDBENCH_QUADTREETEST_H
#define QUADKDBENCH_QUADTREETEST_H


#include <list>
#include "../include/Util.h"
#include "../include/QuadTree.h"
#include "../include/PointRegionQuadTree.h"


class QuadTreeTest {
private:
    void testContainsHelper(QuadTree *quadTree2, PointRegionQuadTree *pointRegionQuadTree);

    QuadTree *getQuadTree();

    PointRegionQuadTree *getPRQuadTree();

public:
    std::list<Point> naiveQuery(std::vector<Point> &points, Area &area);

    static void testQuery();

    static void insertTest();

    static void testContains();
};


#endif //QUADKDBENCH_QUADTREETEST_H
