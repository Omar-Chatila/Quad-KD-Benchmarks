//
// Created by omar on 02.02.24.
//

#ifndef QUADKDBENCH_KDTREETESTS_H
#define QUADKDBENCH_KDTREETESTS_H

#include <list>
#include "../include/Util.h"
#include "../include/KDTreeEfficient.h"

class KDTreeTests {

public:
    static std::list<Point> naiveQuery(std::vector<Point> &points, Area &area);

    static void testQuery();

};


#endif //QUADKDBENCH_KDTREETESTS_H
