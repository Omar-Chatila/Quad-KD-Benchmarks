//
// Created by omarc on 18/01/2024.
//


#include "../../include/KDTreeEfficient.h"
#include "../../include/TreeHelper.h"
#include "spacer.hpp"

#define START 512
#define END 33'554'432

using namespace std;
using namespace util;


static int64_t queryQuadTree(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    QuadTree *quadTree = buildQuadTreeRandom(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    spacer.reset();
    std::list result = quadTree->query(bigArea);
    int64_t spaceUsed = spacer.space_used();
    delete quadTree;
    return spaceUsed;
}

static int64_t queryPrQuadTree(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    PointRegionQuadTree *quadTree = buildPRQuadTreeRandom(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    spacer.reset();
    std::list result = quadTree->query(bigArea);
    int64_t spaceUsed = spacer.space_used();
    delete quadTree;
    return spaceUsed;
}


static int64_t queryKDETree(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    auto kdTreeEfficient = buildEKD_Random(pointNumber);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    spacer.reset();
    kdTreeEfficient->query(bigArea);
    std::list result = kdTreeEfficient->query(bigArea);
    int64_t spaceUsed = spacer.space_used();
    delete kdTreeEfficient;
    return spaceUsed;
}


static int64_t queryMYKDTree(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    auto myKdTree = buildMyKD_Random(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    spacer.reset();
    myKdTree->query(bigArea);
    std::list result = myKdTree->query(bigArea);
    int64_t spaceUsed = spacer.space_used();
    delete myKdTree;
    return spaceUsed;
}


static int64_t queryNaive(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    vector<Point> points = getRandomPoints(pointNumber);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    spacer.reset();
    std::list result = getQueryNaive(points, bigArea);
    int64_t spaceUsed = spacer.space_used();
    return spaceUsed;
}


static int64_t quadTree_contains(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    QuadTree *quadTree = buildQuadTreeRandom(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    searchPoints.reserve(size / 100);
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    spacer.reset();
    qtContainsPoint(quadTree, searchPoints);
    int64_t spaceUsed = spacer.space_used();
    delete quadTree;
    return spaceUsed;
}

static int64_t prQuadTree_contains(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    PointRegionQuadTree *quadTree = buildPRQuadTreeRandom(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    searchPoints.reserve(size / 100);
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    cout << "size " << searchPoints.size() << endl;
    spacer.reset();
    pr_qtContainsPoint(quadTree, searchPoints);
    int64_t spaceUsed = spacer.space_used();
    delete quadTree;
    return spaceUsed;
}

static int64_t kDTreeEfficient_Contains(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    KDTreeEfficient *tree = buildEKD_Random(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    spacer.reset();
    kdEContainsPoint(tree, searchPoints);
    int64_t spaceUsed = spacer.space_used();
    delete tree;
    return spaceUsed;
}

static int64_t myKDTree_Contains(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    MyKDTree *tree = buildMyKD_Random(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    spacer.reset();
    myKdContainsPoint(tree, searchPoints);
    int64_t spaceUsed = spacer.space_used();
    delete tree;
    return spaceUsed;
}

static int64_t naive_Contains(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    spacer.reset();
    containsNaive(searchPoints, points);
    int64_t spaceUsed = spacer.space_used();
    return spaceUsed;
}

static int64_t quadTree_NNS(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    QuadTree *quadTree = buildQuadTreeRandom(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    searchPoints.reserve(size / 100);
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }

    Point query{0.4 * pointNumber, 0.7 * pointNumber};
    spacer.reset();
    vector<Point> result = quadTree->kNearestNeighbors(query, 10);
    int64_t spaceUsed = spacer.space_used();
    delete quadTree;
    return spaceUsed;
}

static int64_t kDTreeEfficient_NNS(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    KDTreeEfficient *tree = buildEKD_Random(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    Point query{0.4 * pointNumber, 0.7 * pointNumber};
    spacer.reset();
    vector<Point> result = tree->kNearestNeighbors(query, 10);
    int64_t spaceUsed = spacer.space_used();
    delete tree;
    return spaceUsed;
}

static int64_t myKDTree_NNS(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    auto tree = buildMyKD_Random(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    Point query{0.4 * pointNumber, 0.7 * pointNumber};
    spacer.reset();
    vector<Point> result = tree->kNearestNeighbors(query, 10);
    int64_t spaceUsed = spacer.space_used();
    delete tree;
    return spaceUsed;
}

static int64_t naive_NNS(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    Point query{0.4 * pointNumber, 0.7 * pointNumber};
    spacer.reset();
    vector<Point> result = naive_kNNS(query, points, 10);
    int64_t spaceUsed = spacer.space_used();
    return spaceUsed;
}

static void startBuildBenchmarks() {
    util::spacer spacer{};
    vector<string> results;

    cout << "Build results in kBytes" << endl;

    /*for (int i = START; i < END; i *= 2) {
        vector<Point> pointVector = getRandomPoints(i);
        double bounds = i;
        Area area{0, bounds, 0, bounds};

        spacer.reset();
        auto quadTree = new QuadTree(area, pointVector);
        quadTree->buildTree();
        int64_t space_in_bytes = spacer.space_used();

        int64_t memory = space_in_bytes / 1024;
        cout << "Build-Quadtree/" + to_string(i) + ": " + to_string(memory) + " kB" + " H: " +
                to_string(quadTree->getHeight()) << "\n";
        delete quadTree;
    }

    results.emplace_back("----------------------------------------------------------------");
    spacer.reset();
    */

    for (int i = START; i < END; i *= 2) {
        vector<Point> pointVector = getRandomPoints(i);
        double bounds = i;
        Area area{0, bounds, 0, bounds};
        int capacity = (int) max(log10(i), 4.0);
        spacer.reset();
        auto prQuadTree = new PointRegionQuadTree(area, pointVector, capacity);
        prQuadTree->buildTree();
        int64_t space_in_bytes = spacer.space_used();

        int64_t memory = space_in_bytes / 1024;
        cout << ("Build-PR-Quadtree/" + to_string(i) + ": " + to_string(memory) + " kB" + " H: " +
                 to_string(prQuadTree->getHeight())) << endl;
        delete prQuadTree;
    }

    results.emplace_back("----------------------------------------------------------------");
    spacer.reset();

    for (int i = START; i < END; i *= 2) {
        auto *pointVector = getRandomPointsArray(i);
        double bounds = i;
        Area area{0, bounds, 0, bounds};
        spacer.reset();
        auto kdTreeEfficient = new KDTreeEfficient(pointVector, 0, area, 0, i);
        kdTreeEfficient->buildTree();
        int64_t space_in_bytes = spacer.space_used();
        int64_t memory = space_in_bytes / 1024;
        results.push_back("Build-KD-Efficient/" + to_string(i) + ": " + to_string(memory) + " kB" + " H: " +
                          to_string(kdTreeEfficient->getHeight()));
        delete kdTreeEfficient;
    }

    results.emplace_back("----------------------------------------------------------------");
    spacer.reset();

    for (int i = START; i < END; i *= 2) {
        vector<Point> pointVector = getRandomPoints(i);
        double bounds = i;
        Area area{0, bounds, 0, bounds};
        spacer.reset();
        auto myKdTree = new MyKDTree(pointVector, area, 0);
        myKdTree->buildTree();
        int64_t space_in_bytes = spacer.space_used();
        int64_t memory = space_in_bytes / 1024;
        results.push_back("Build-My-KDTree/" + to_string(i) + ": " + to_string(memory) + " kB. " + " H: " +
                          to_string(myKdTree->getHeight()));
        delete myKdTree;
    }

    for (const auto &record: results) {
        cout << record << "\n";
    }
}

static void queryBenchmarks() {
    util::spacer spacer{};
    vector<string> results;

    cout << "Query-Results in Bytes" << endl;

    for (int i = START; i < END; i *= 2) {
        int64_t space = queryQuadTree(i, spacer);
        cout << ("Quadtree-Query/" + to_string(i) + ": " + to_string(space)) << endl;
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = queryPrQuadTree(i, spacer);
        cout << ("PRQuadtree-Query/" + to_string(i) + ": " + to_string(space)) << endl;
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = queryKDETree(i, spacer);
        cout << ("EKD-Query/" + to_string(i) + ": " + to_string(space)) << endl;
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = queryMYKDTree(i, spacer);
        cout << ("MKD-Query/" + to_string(i) + ": " + to_string(space)) << endl;
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = queryNaive(i, spacer);
        cout << ("Naive-Query/" + to_string(i) + ": " + to_string(space)) << endl;
    }

    for (const auto &record: results) {
        // cout << record << "\n";
    }
}

static void containsBenchmarks() {
    util::spacer spacer{};
    vector<string> results;
    for (int i = START; i < END; i *= 2) {
        int64_t space = quadTree_contains(i, spacer);
        cout << ("Quadtree-Contains/" + to_string(i) + ": " + to_string(space)) << endl;
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = prQuadTree_contains(i, spacer);
        cout << ("PR-Quadtree-Contains/" + to_string(i) + ": " + to_string(space)) << endl;
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = kDTreeEfficient_Contains(i, spacer);
        cout << ("EKD-Contains/" + to_string(i) + ": " + to_string(space)) << endl;
    }
    for (int i = START; i < END; i *= 2) {
        int64_t space = myKDTree_Contains(i, spacer);
        cout << ("MKD-Contains/" + to_string(i) + ": " + to_string(space)) << endl;
    }
    for (int i = START; i < END; i *= 2) {
        int64_t space = naive_Contains(i, spacer);
        cout << ("Naive-Contains/" + to_string(i) + ": " + to_string(space)) << endl;
    }
    for (const auto &record: results) {
        // cout << record << "\n";
    }
}

static void kNNSBenchmarks() {
    util::spacer spacer{};
    vector<string> results;

    cout << "k-NNS-Results in Bytes" << endl;

    for (int i = START; i < END; i *= 2) {
        int64_t space = quadTree_NNS(i, spacer);
        results.push_back("Quadtree-NNS/" + to_string(i) + ": " + to_string(space));
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = kDTreeEfficient_NNS(i, spacer);
        results.push_back("EKD-NNS/" + to_string(i) + ": " + to_string(space));
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = myKDTree_NNS(i, spacer);
        results.push_back("MKD-NNS/" + to_string(i) + ": " + to_string(space));
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = naive_NNS(i, spacer);
        results.push_back("Naive-NNS/" + to_string(i) + ": " + to_string(space));
    }

    for (const auto &record: results) {
        cout << record << "\n";
    }
}

int main() {
    /*cout << "++++++++++++++++++START BUILD BENCHMARKS++++++++++++++++++" << "\n";
    startBuildBenchmarks();

    cout << "++++++++++++++++++START NNS BENCHMARKS++++++++++++++++++" << "\n";
    kNNSBenchmarks();
    cout << "++++++++++++++++++START QUERY BENCHMARKS++++++++++++++++++" << "\n";
    queryBenchmarks();
    */
    cout << "++++++++++++++++++START CONTAINS BENCHMARKS++++++++++++++++++" << "\n";
    containsBenchmarks();
    return 0;
}


