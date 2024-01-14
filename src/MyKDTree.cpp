//
// Created by omarc on 12/01/2024.
//

#include "../include/MyKDTree.h"

MyKDTree::MyKDTree(vector<Point> &points, Area area, int level) {
    this->points = points;
    this->area = area;
    this->level = level;
    if (level % 2 == 0) {
        sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
            return a.x < b.x;
        });
    } else {
        sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
            return a.y < b.y;
        });
    }
}

void MyKDTree::buildTree(int lev) {
    if (this->points.size() > 1) {
        // vertical split
        if (level % 2 == 0) {
            this->setVerticalChildren(lev);
        } else {
            // horizontal split
            this->setHorizontalChildren(lev);
        }
        leftChild->buildTree(lev + 1);
        rightChild->buildTree(lev + 1);
    }
}

void MyKDTree::buildTree() {
    buildTree(0);
}

void MyKDTree::setVerticalChildren(int lev) {
    std::vector<std::vector<Point>> splitVectors = splitVector(points);
    Area leftArea = Area(this->area.xMin, getMedian(points, true), this->area.yMin, this->area.yMax);
    this->leftChild = new MyKDTree(splitVectors.at(0), leftArea, lev + 1);
    Area rightArea = Area(getMedian(points, true), this->area.xMax, this->area.yMin, this->area.yMax);
    this->rightChild = new MyKDTree(splitVectors.at(1), rightArea, lev + 1);
}

void MyKDTree::setHorizontalChildren(int lev) {
    std::vector<std::vector<Point>> splitVectors = splitVector(points);
    Area lowerArea = Area(this->area.xMin, this->area.xMax, this->area.yMin, getMedian(points, false));
    this->leftChild = new MyKDTree(splitVectors.at(0), lowerArea, lev + 1);
    Area higherArea = Area(this->area.xMin, this->area.xMax, getMedian(points, false), this->area.yMax);
    this->rightChild = new MyKDTree(splitVectors.at(1), higherArea, lev + 1);
}

bool MyKDTree::contains(Point point) {
    MyKDTree *current = this;
    while (!current->isLeaf()) {
        if (current->level % 2 == 0) {
            current = current->verticalSplitLine.fromX >= point.x ? current->leftChild : current->rightChild;
        } else {
            current = current->horizontalSplitLine.fromY >= point.y ? current->leftChild : current->rightChild;
        }
    }
    return std::find(current->points.begin(), current->points.end(), point) != current->points.end();
}

vector<Point> MyKDTree::query(Area queryRectangle) {
    vector<Point> result = vector<Point>();
    if (this->isLeaf()) {
        if (containsPoint(queryRectangle, this->points.at(0))) {
            result.push_back(this->points.at(0));
        }
    } else if (containsArea(queryRectangle, this->area)) {
        result.insert(result.end(), this->points.begin(), this->points.end());
    }
    if (this->leftChild != nullptr && intersects(queryRectangle, this->leftChild->area)) {
        vector<Point> childResult = this->leftChild->query(queryRectangle);
        result.insert(result.end(), childResult.begin(), childResult.end());
    }
    if (this->rightChild != nullptr && intersects(queryRectangle, this->rightChild->area)) {
        vector<Point> childResult = this->rightChild->query(queryRectangle);
        result.insert(result.end(), childResult.begin(), childResult.end());
    }
    return result;
}

int MyKDTree::getHeight() {
    if (isLeaf()) {
        return 1;
    }
    int h1 = 0, h2 = 0;
    if (leftChild != nullptr)
        h1 = this->leftChild->getHeight();
    if (rightChild != nullptr)
        h2 = this->rightChild->getHeight();
    if (h1 > h2) {
        return h1 + 1;
    } else {
        return h2 + 1;
    }
}

bool MyKDTree::isLeaf() {
    return this->points.size() == 1;
}

void MyKDTree::setSplitLines() {
    if (level % 2 == 0) {
        double xMedian = getMedian(points, true);
        verticalSplitLine = SplitLine(xMedian, area.yMin, xMedian, area.yMax);
    } else {
        double yMedian = getMedian(points, false);
        horizontalSplitLine = SplitLine(area.xMin, yMedian, area.xMax, yMedian);
    }
}

bool MyKDTree::isEmpty() {
    return this->points.empty();
}

void MyKDTree::add(Point point) {
    MyKDTree *current = this;
    int lev = 0;
    while (!current->isLeaf()) {
        if ((lev++ % 2) == 0) {
            if (point.x <= current->verticalSplitLine.toX) {
                current = current->leftChild;
            } else {
                current = current->rightChild;
            }
        } else {
            if (point.y <= current->horizontalSplitLine.toY) {
                current = current->leftChild;
            } else {
                current = current->rightChild;
            }
        }
    }
    current->appendPoint(point, lev);
    if (lev % 2 == 0) {
        current->setVerticalChildren(lev);
    } else {
        current->setHorizontalChildren(lev);
    }
}

void MyKDTree::appendPoint(Point point, int level) {
    points.push_back(point);
    if (level % 2 == 0) {
        sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
            return a.x < b.x;
        });

    } else {
        sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
            return a.y < b.y;
        });

    }
    setSplitLines();
}
