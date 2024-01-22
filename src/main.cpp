#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/KDTreeEfficient.h"
#include "../include/TreeHelper.h"
#include "../include/QuadTree.h"
#include "../include/MyKDTree.h"
#include "../include/Util.h"
#include "spacer/spacer.hpp"

#define FAST_IO() ios_base::sync_with_stdio(false); cin.tie(NULL)

using namespace util;

static string treeToString(KDTreeEfficient *node) {
    std::ostringstream oss;
    oss << *node;
    std::string result = oss.str();
    if (node->getLeftChild() != nullptr) {
        result = result + treeToString(node->getLeftChild());
    }
    if (node->getRightChild() != nullptr) {
        result = result + treeToString(node->getRightChild());
    }
    return result;
}

static void writeStringToFile(string &result, string &outputPath) {
    std::ofstream outputFile(outputPath);
    if (outputFile.is_open()) {
        outputFile << result;
        outputFile.close();
        std::cout << "Result written to: " << outputPath << std::endl;
    } else {
        std::cerr << "Unable to open the output file!" << std::endl;
    }
}

static void writeTreeToFile(KDTreeEfficient &kdTreeEfficient) {
    std::ostringstream oss;
    oss << treeToString(&kdTreeEfficient);
    std::string result = oss.str();

    std::string outputPath = R"(C:\Users\omarc\CLionProjects\QuadKDBench\kdEBuildOutput.txt)";

    writeStringToFile(result, outputPath);
}

static void testKDE_Query() {
    KDTreeEfficient kdTreeEfficient = buildEKD_Random(100000);
    list<Point> queryResult = testQuery(kdTreeEfficient);

    std::string outputPath = R"(C:\Users\omarc\CLionProjects\QuadKDBench\queryOutput.txt)";

    std::ostringstream oss;
    for (auto point: queryResult) {
        oss << point << "\n";
    }

    string result = oss.str();
    writeStringToFile(result, outputPath);
}

static string qtToString(QuadTree *node) {
    std::ostringstream oss;
    oss << *node;
    std::string result = oss.str();
    if (node->getNorthEast() != nullptr) {
        result = result + qtToString(node->getNorthEast());
    }
    if (node->getNorthWest() != nullptr) {
        result = result + qtToString(node->getNorthWest());
    }
    if (node->getSouthWest() != nullptr) {
        result = result + qtToString(node->getSouthWest());
    }
    if (node->getSouthEast() != nullptr) {
        result = result + qtToString(node->getSouthEast());
    }
    return result;
}

static void writeQTtoFile(QuadTree &quadTree) {
    string treeString = qtToString(&quadTree);
    std::string outputPath = R"(C:\Users\omarc\CLionProjects\QuadKDBench\qtBuildOutput.txt)";
    writeStringToFile(treeString, outputPath);
}

static void testQT_Query() {
    vector<Point> points = getRandomPoints(100000);
    cout << points.size() << "\n";
    Area area{0, 10000, 0, 10000};
    QuadTree quadTree(area, points);
    quadTree.buildTree();

    //writeQTtoFile(quadTree);

    Area queryArea{234, 7000, 2000, 9000};

    list<Point> queried = quadTree.query(queryArea);
    std::ostringstream oss;
    for (auto point: queried) {
        oss << point << "\n";
    }
    std::string result = oss.str();

    string outputPath = R"(C:\Users\omarc\CLionProjects\QuadKDBench\qtQueryOutput.txt)";
    writeStringToFile(result, outputPath);
}

static void testMyKDTree() {
    vector<Point> points = getRandomPoints(100000);
    cout << points.size() << "\n";
    Area area{0, 10000, 0, 10000};

    Area queryArea{234, 7000, 2000, 9000};

    int actual = 0;

    cout << "actual: " << "\n";
    for (auto point: points) {
        if (containsPoint(queryArea, point)) {
            actual++;
        }
    }
    cout << actual << "\n";

    MyKDTree myKdTree(points, area, 0);
    myKdTree.buildTree();

    list<Point> queried = myKdTree.query(queryArea);
    cout << "queried: " << "\n";
    int queriedNo = 0;
    std:
    ostringstream oss;
    for (auto point: queried) {
        oss << point << "\n";
        queriedNo++;
    }
    cout << "Actual " << actual << ". queried " << queriedNo << endl;

    string queryResult = oss.str();
    string outputPath = R"(C:\Users\omarc\CLionProjects\QuadKDBench\myKDQueryOutput.txt)";
    writeStringToFile(queryResult, outputPath);
}

int main(int argc, char *argv[]) {
    FAST_IO();
    int n = atoi(argv[1]);
    double bounds = n;
    int k = atoi(argv[2]);
    cout << "Nearest-Neighbor-Search - k: " << to_string(k) << " n: " << to_string(n) << endl;

    Area area{0, bounds, 0, bounds};
    vector<Point> points = getRandomPoints(n);
    QuadTree quadTree(area, points);
    quadTree.buildTree();
    Point p{0.3 * bounds, 0.8 * bounds};

    auto start = std::chrono::high_resolution_clock::now();
    vector<Point> nns = quadTree.kNearestNeighbors(p, k);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by QuadTree: " << duration_us.count() << " µs" << std::endl;

    for (auto point: nns) {
        cout << point << endl;
    }

    cout << "Naive" << "\n";

    auto startN = std::chrono::high_resolution_clock::now();
    vector<Point> naiveResult = naive_kNNS(p, points, k);
    auto stopN = std::chrono::high_resolution_clock::now();
    auto duration_usN = std::chrono::duration_cast<std::chrono::microseconds>(stopN - startN);
    std::cout << "Time taken by Naive: " << duration_usN.count() << " µs" << std::endl;

    for (auto point: naiveResult) {
        cout << point << endl;
    }

    Point *pointsArray = (Point *) (malloc(n * sizeof(Point)));
    int i = 0;
    for (auto point: points) {
        pointsArray[i++] = point;
    }
    KDTreeEfficient kdTreeEfficient(pointsArray, 0, area, 0, n - 1);
    kdTreeEfficient.buildTree();

    auto start3 = std::chrono::high_resolution_clock::now();
    vector<Point> nns2 = kdTreeEfficient.kNearestNeighbors(p, k);
    auto stop3 = std::chrono::high_resolution_clock::now();
    auto duration_us3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);
    std::cout << "Time taken by KD-Tree_Efficient: " << duration_us3.count() << " µs" << std::endl;

    for (auto point: nns2) {
        cout << point << "\n";
    }

    return 0;
}



