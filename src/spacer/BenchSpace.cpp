//
// Created by omarc on 18/01/2024.
//


#include "../../include/KDTreeEfficient.h"
#include "../../include/TreeHelper.h"
#include "spacer.hpp"

#define START 512
#define END 2'097'153
#define ITERATIONS 100

using namespace std;
using namespace util;


static int64_t queryQuadTree(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    QuadTree quadTree = buildQuadTreeRandom(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    spacer.reset();
    std::list result = quadTree.query(bigArea);
    int64_t spaceUsed = spacer.space_used();
    result.clear();
    return spaceUsed;
}


static int64_t queryKDETree(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    KDTreeEfficient kdTreeEfficient = buildEKD_Random(pointNumber);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    spacer.reset();
    kdTreeEfficient.query(bigArea);
    std::list result = kdTreeEfficient.query(bigArea);
    int64_t spaceUsed = spacer.space_used();
    result.clear();
    return spaceUsed;
}


static int64_t queryMYKDTree(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    MyKDTree myKdTree = buildMyKD_Random(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    spacer.reset();
    myKdTree.query(bigArea);
    std::list result = myKdTree.query(bigArea);
    int64_t spaceUsed = spacer.space_used();
    result.clear();
    return spaceUsed;
}


static int64_t queryNaive(int pointNumber, spacer &spacer) {
    int size = pointNumber;
    vector<Point> points = getRandomPoints(pointNumber);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    spacer.reset();
    std::list result = getQueryNaive(points, bigArea);
    int64_t spaceUsed = spacer.space_used();
    result.clear();
    return spaceUsed;
}


static void quadTree_contains(int pointNumber) {
    int size = pointNumber;
    QuadTree quadTree = buildQuadTreeRandom(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    searchPoints.reserve(size / 100);
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    qtContainsPoint(quadTree, searchPoints);
}

static void kDTreeEfficient_Contains(int pointNumber) {
    int size = pointNumber;
    KDTreeEfficient tree = buildEKD_Random(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }

    kdEContainsPoint(tree, searchPoints);
}

static void myKDTree_Contains(int pointNumber) {
    int size = pointNumber;
    MyKDTree tree = buildMyKD_Random(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }

    myKdContainsPoint(tree, searchPoints);

}

static void naive_Contains(int pointNumber) {
    int size = pointNumber;
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < pointNumber; i += step) {
        searchPoints.push_back(points.at(i));
    }
    containsNaive(searchPoints, points);
}

static void startBuildBenchmarks() {
    util::spacer spacer{};
    vector<string> results;

    cout << "Buidl results in kBytes" << endl;

    for (int i = START; i < END; i *= 2) {
        vector<Point> pointVector = getRandomPoints(i);
        double bounds = i;
        Area area{0, bounds, 0, bounds};

        spacer.reset();
        QuadTree quadTree(area, pointVector);
        quadTree.buildTree();
        int64_t space_in_bytes = spacer.space_used();

        int64_t memory = space_in_bytes / 1024;
        results.push_back("Build-Quadtree/" + to_string(i) + ": " + to_string(memory) + " kB" + " H: " +
                          to_string(quadTree.getHeight()));
    }

    results.emplace_back("----------------------------------------------------------------");
    spacer.reset();

    for (int i = START; i < END; i *= 2) {
        auto *pointVector = getRandomPointsArray(i);
        double bounds = i;
        Area area{0, bounds, 0, bounds};
        spacer.reset();
        KDTreeEfficient kdTreeEfficient(pointVector, 0, area, 0, i);
        kdTreeEfficient.buildTree();
        int64_t space_in_bytes = spacer.space_used();
        int64_t memory = space_in_bytes / 1024;
        results.push_back("Build-KD-Efficient/" + to_string(i) + ": " + to_string(memory) + " kB" + " H: " +
                          to_string(kdTreeEfficient.getHeight()));
    }

    results.emplace_back("----------------------------------------------------------------");
    spacer.reset();

    for (int i = START; i < END; i *= 2) {
        vector<Point> pointVector = getRandomPoints(i);
        double bounds = i;
        Area area{0, bounds, 0, bounds};
        spacer.reset();
        MyKDTree myKdTree(pointVector, area, 0);
        myKdTree.buildTree();
        int64_t space_in_bytes = spacer.space_used();
        int64_t memory = space_in_bytes / 1024;
        results.push_back("Build-My-KDTree/" + to_string(i) + ": " + to_string(memory) + " kB. " + " H: " +
                          to_string(myKdTree.getHeight()));
    }

    for (const auto &record: results) {
        cout << record << "\n";
    }
}

static void startQueryBenchmarks() {
    util::spacer spacer{};
    vector<string> results;

    cout << "Query-Results in Bytes" << endl;

    for (int i = START; i < END; i *= 2) {
        int64_t space = queryQuadTree(i, spacer);
        results.push_back("Quadtree-Query/" + to_string(i) + ": " + to_string(space));
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = queryKDETree(i, spacer);
        results.push_back("EKD-Query/" + to_string(i) + ": " + to_string(space));
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = queryMYKDTree(i, spacer);
        results.push_back("MKD-Query/" + to_string(i) + ": " + to_string(space));
    }

    for (int i = START; i < END; i *= 2) {
        int64_t space = queryNaive(i, spacer);
        results.push_back("Naive-Query/" + to_string(i) + ": " + to_string(space));
    }

    for (const auto &record: results) {
        cout << record << "\n";
    }
}

int main() {
    startQueryBenchmarks();
    startBuildBenchmarks();
    return 0;
}

