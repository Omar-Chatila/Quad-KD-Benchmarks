#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/KDTreeEfficient.h"
#include "../include/TreeHelper.h"
#include "../include/QuadTree.h"
#include "../include/MyKDTree.h"

#define FAST_IO() ios_base::sync_with_stdio(false); cin.tie(NULL)
#pragma GCC optimize("O3")
#pragma comment(linker, "/STACK:1000000000")

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
    KDTreeEfficient kdTreeEfficient = buildEKDTreeFromFile(100000);
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

int main() {
    FAST_IO();

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
    for (auto point: queried) {
        queriedNo++;
    }
    cout << "Actual " << actual << ". queried " << queriedNo << endl;

    return 0;
}



