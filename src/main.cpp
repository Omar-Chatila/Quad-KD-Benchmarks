#include <iostream>
#include "../include/KDTreeEfficient.h"
#include "../include/TreeHelper.h"
#include "spacer/spacer.hpp"

#define FAST_IO() ios_base::sync_with_stdio(false); cin.tie(NULL)

using namespace util;

int main(int argc, char *argv[]) {
    FAST_IO();
    Point *points = getRandomPointsArray(100000);
    Area area{0, 100000, 0, 100000};
    auto *kdTreeEfficient = new KDTreeEfficient(points, area);
    kdTreeEfficient->buildTree();
    cout << points[234] << endl;
    cout << sizeof(points) << endl;
    delete kdTreeEfficient;
    cout << "hier" << endl;
    return 0;
}



