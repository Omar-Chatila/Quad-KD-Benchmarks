#include <iostream>
#include "../include/KDTreeEfficient.h"
#include "../include/TreeHelper.h"
#include "spacer/spacer.hpp"

#define FAST_IO() ios_base::sync_with_stdio(false); cin.tie(NULL)

using namespace util;

int main(int argc, char *argv[]) {
    FAST_IO();
    vector<Point> points = getRandomPoints(10000);
    double bounds = 10000;
    Area area{0, bounds, 0, bounds};
    auto *myKdTree = new SortKDTree(points, area, 0);
    myKdTree->buildTree();
    cout << "hier" << endl;
    delete myKdTree;
    cout << "hier" << endl;
    return 0;
}



