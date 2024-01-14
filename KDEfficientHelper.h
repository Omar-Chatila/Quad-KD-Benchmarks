//
// Created by omarc on 13/01/2024.
//

#ifndef QUADKDBENCH_KDEFFICIENTHELPER_H
#define QUADKDBENCH_KDEFFICIENTHELPER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "KDTreeEfficient.h"
#include "Util.h"

using namespace std;

class KDEfficientHelper {
public:
    static KDTreeEfficient buildTreeFromFile(int pointNumber);

    static list<Point> testQuery(KDTreeEfficient &kdTreeEfficient);
};


#endif //QUADKDBENCH_KDEFFICIENTHELPER_H
