#include <iostream>
#include <fstream>
#include <sstream>
#include "QuadTree.h"
#include "MyKDTree.h"
#include "KDTreeEfficient.h"

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


int main() {
    FAST_IO();

    std::ifstream inputFile(R"(C:\Users\omarc\CLionProjects\QuadKDBench\random_points.txt)");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    int pointNumber = 100000;

    std::vector<Point> points;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string token;

        // Split the line into tokens using comma as the delimiter
        getline(iss, token, ',');
        double x = std::stod(token);

        getline(iss, token, ',');
        double y = std::stod(token);

        points.emplace_back(x, y);
    }
    inputFile.close();

    Point pointArray[100000];

    int i = 0;
    for (auto point: points) {
        pointArray[i++] = point;
    }

    Area area{0, 10000, 0, 10000};
    KDTreeEfficient kdTreeEfficient(pointArray, 0, area, 0, pointNumber - 1);
    kdTreeEfficient.buildTree();

    cout << kdTreeEfficient.getHeight() << endl;

    std::ostringstream oss;
    oss << treeToString(&kdTreeEfficient);
    std::string result = oss.str(); // write this in output.txt like you did in Java

    std::string outputPath = R"(C:\Users\omarc\CLionProjects\QuadKDBench\output.txt)";

    std::ofstream outputFile(outputPath);
    if (outputFile.is_open()) {
        outputFile << result;
        outputFile.close();
        std::cout << "Result written to: " << outputPath << std::endl;
    } else {
        std::cerr << "Unable to open the output file!" << std::endl;
    }

    return 0;

    return 0;
}

