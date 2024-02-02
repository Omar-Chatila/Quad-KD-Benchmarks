//
// Created by omar on 02.02.24.
//

#include "Tests.h"
#include "QuadTreeTest.h"
#include "KDTreeTests.h"
#include "UtilTest.h"

int main() {
    QuadTreeTest::testContains();
    QuadTreeTest::testQuery();
    QuadTreeTest::insertTest();

    KDTreeTests::testQuery();

    UtilTest::containsAreaTest();
    UtilTest::containsPointTest();
    UtilTest::intersectsTest();
    UtilTest::splitTest();
    UtilTest::sqDistanceFromTest();
    UtilTest::pointDistanceTest();

    cout << "All tests passed" << endl;
    return 0;
}

