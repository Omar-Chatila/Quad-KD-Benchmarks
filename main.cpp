#include <iostream>
#include <fstream>
#include <sstream>
#include "KDTreeEfficient.h"
#include "KDEfficientHelper.h"

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

int main() {
    FAST_IO();
    KDTreeEfficient kdTreeEfficient = KDEfficientHelper::buildTreeFromFile(100000);
    list<Point> queryResult = KDEfficientHelper::testQuery(kdTreeEfficient);

    std::string outputPath = R"(C:\Users\omarc\CLionProjects\QuadKDBench\queryOutput.txt)";

    std::ostringstream oss;
    for (auto point: queryResult) {
        oss << point << "\n";
    }

    string result = oss.str();
    writeStringToFile(result, outputPath);
    return 0;
}



