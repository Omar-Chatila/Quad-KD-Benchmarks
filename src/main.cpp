#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/KDTreeEfficient.h"
#include "../include/TreeHelper.h"
#include "../include/PointRegionQuadTree.h"
#include "../include/KDBTreeEfficient.h"
#include "../include/QuadTree.h"
#include "../include/MyKDTree.h"
#include "../include/Util.h"
#include "spacer/spacer.hpp"

#define FAST_IO() ios_base::sync_with_stdio(false); cin.tie(NULL)

using namespace util;

int main(int argc, char *argv[]) {
    FAST_IO();
    Area area{0, 100, 0, 100};
    Point *pointsA = getRandomPointsArray(100);
    auto *kdbTreeEfficient = new KDBTreeEfficient(pointsA, 0, area, 0, 100, 4);
    kdbTreeEfficient->buildTree();
    cout << kdbTreeEfficient->getHeight() << endl;
    Point p{54, 34};
    vector<Point> result = kdbTreeEfficient->kNearestNeighbors(p, 4);
    for (auto point: result) {
        cout << point << endl;
    }
    return 0;
}



