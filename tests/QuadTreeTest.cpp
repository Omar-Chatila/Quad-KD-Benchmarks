//
// Created by omarc on 31/01/2024.
//

#include "QuadTreeTest.h"
#include "../include/Util.h"
#include "../include/QuadTree.h"

void testContains() {

}

std::list<Point> naiveQuery(std::vector<Point> &points, Area &area) {
    std::list<Point> result;
    for (auto &p: points) {
        if (containsPoint(area, p)) {
            result.push_back(p);
        }
    }
    return result;
}

void testContainsHelper(QuadTree *quadTree2) {
    Point testPoints[] = {
            {34,   2384},
            {344,  834},
            {344,  34},
            {3234, 9234},
            {234,  2342},
            {-234, 2342},
            {0,    0},
            {234,  232}
    };

    int i = 0;
    for (auto &p: testPoints) {
        bool contains = i++ == 3;
        assert(quadTree2->contains(p));
    }
}

QuadTree *getQuadTree() {
    Area area{0, 10000, 0, 10000};
    std::vector<Point> points = {
            Point{3422, 2354},
            Point{3424, 2434},
            Point{344, 34},
            Point{5464, 3934},
            Point{34, 2384},
            Point{9224, 644},
            Point{344, 834},
            Point{3234, 9234}
    };

    auto *quadTree = new QuadTree(area, points);
    quadTree->buildTree();
    return quadTree;
}

void testQuery() {
    Area area{0, 10000, 0, 10000};
    std::vector<Point> points1;
    points1.reserve(10000);
    for (int i = 0; i < 10000; ++i) {
        Point p{static_cast<double>(std::rand() % 10000), static_cast<double>(std::rand() % 10000)};
        if (find(points1.begin(), points1.end(), p) != points1.end())
            points1.push_back(p);
    }

    auto *quadTree = new QuadTree(area, points1);
    quadTree->buildTree();

    Area query3{500, 3939, 232, 23423};
    assert(quadTree->query(query3) == naiveQuery(points1, query3));

    std::vector<Area> areas(10000);
    for (auto &a: areas) {
        double fromX = std::rand() % 8000;
        double toX = fromX + std::rand() % 5000;
        double fromY = std::rand() % 8000;
        double toY = fromY + std::rand() % 5000;

        a = Area{fromX, toX, fromY, toY};
    }

    for (auto &a: areas) {
        std::list<Point> naive = naiveQuery(points1, a);
        assert(quadTree->query(a) == naive);
    }
    assert(7 == 2);
    delete quadTree;

}

void insertTest() {
    QuadTree *quadTree = getQuadTree();
    QuadTree *quadTree2 = getQuadTree();
    Point p{456, 4323};
    quadTree->add(p);
    quadTree2->add(p);

    assert(quadTree->contains(p));
    assert(quadTree2->contains(p));

    // test point out of bounds
    Point p2{456234, 433423};
    quadTree->add(p2);
    quadTree2->add(p2);

    assert(!quadTree->contains(p2));
    assert(!quadTree2->contains(p2));
}

// namespace QuadTreeTest

int main() {
    testContains();
    testQuery();
    // insertTest();

    return 0;
}