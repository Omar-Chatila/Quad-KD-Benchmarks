#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/KDTreeEfficient.h"
#include "../include/KDEfficientHelper.h"
#include "../include/QuadTree.h"

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
    KDTreeEfficient kdTreeEfficient = buildTreeFromFile(100000);
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

int main() {
    FAST_IO();
    vector<Point> points = getRandomPoints(10);
    Area area{0, 10000, 0, 10000};
    QuadTree quadTree(area, points);
    quadTree.buildTree();

    string treeString = qtToString(&quadTree);
    std::string outputPath = R"(C:\Users\omarc\CLionProjects\QuadKDBench\qtBuildOutput.txt)";

    writeStringToFile(treeString, outputPath);

    return 0;
}



