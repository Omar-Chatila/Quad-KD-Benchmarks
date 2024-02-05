//
// Created by omar on 02.02.24.
//

#include <cassert>
#include "../include/Util.h"
#include "UtilTest.h"

void UtilTest::splitTest() {
    Area area(0, 10, 0, 10);
    Area *subAreas = splitArea(area, 5.0, 5.0);

    Area ne(5, 10, 5, 10);
    Area nw(0, 5, 5, 10);
    Area sw(0, 5, 0, 5);
    Area se(5, 10, 0, 5);

    assert(ne == subAreas[NORTH_EAST]);
    assert(nw == subAreas[NORTH_WEST]);
    assert(sw == subAreas[SOUTH_WEST]);
    assert(se == subAreas[SOUTH_EAST]);

}

void UtilTest::intersectsTest() {
    Area area1(0, 10, 0, 10);
    Area area2(5, 15, 5, 15);
    assert(intersects(area1, area2));

    Area area3(20, 30, 20, 30);
    assert(!intersects(area1, area3));
}

void UtilTest::containsAreaTest() {
    Area area1(0, 10, 0, 10);
    Area area2(2, 8, 2, 8);
    assert(containsArea(area1, area2));

    Area area3(12, 15, 12, 15);
    Area area4(0, 10, 0, 25);
    assert(!containsArea(area1, area3));
    assert(!containsArea(area1, area4));
}

void UtilTest::containsPointTest() {
    Area area(0, 10, 0, 10);
    Point insidePoint(5, 5);
    Point outsidePoint(15, 15);

    assert(containsPoint(area, insidePoint));
    assert(!containsPoint(area, outsidePoint));
}

void UtilTest::sqDistanceFromTest() {
    Area area(0, 10, 0, 10);
    Point pointInside(5, 5);
    Point pointOutside(15, 15);

    assert(0.0 == sqDistanceFrom(area, pointInside));
    assert(50.0 == sqDistanceFrom(area, pointOutside));
}

void UtilTest::pointDistanceTest() {
    Point p{2, 3};
    Point q{4, 2};

    assert(pointDistance(p, q) == 5);
}
