//
// Created by omarc on 12/01/2024.
//

#include "../include/MyKDTree.h"

MyKDTree::MyKDTree(vector<Point> &points, Area &area, int level) {
    this->points = points;
    this->area = area;
    this->level = level;
    if (level % 2 == 0) {
        sort(this->points.begin(), this->points.end(), [](const Point &a, const Point &b) {
            return a.x < b.x;
        });
    } else {
        sort(this->points.begin(), this->points.end(), [](const Point &a, const Point &b) {
            return a.y < b.y;
        });
    }
}


MyKDTree::~MyKDTree() {
    this->points.clear();
    delete leftChild;
    delete rightChild;
}

void MyKDTree::deleteTree(MyKDTree *node) {
    if (node != nullptr) {
        deleteTree(node->leftChild);
        deleteTree(node->rightChild);
        delete node;
    }
}

void MyKDTree::buildTree(int lev) {
    if (this->points.size() > 1) {
        // vertical split
        if (lev % 2 == 0) {
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
    Area leftArea = Area{this->area.xMin, getMedian(points, true), this->area.yMin, this->area.yMax};
    this->leftChild = new MyKDTree(splitVectors[0], leftArea, lev + 1);
    Area rightArea = Area{getMedian(points, true), this->area.xMax, this->area.yMin, this->area.yMax};
    this->rightChild = new MyKDTree(splitVectors[1], rightArea, lev + 1);
}

void MyKDTree::setHorizontalChildren(int lev) {
    std::vector<std::vector<Point>> splitVectors = splitVector(points);
    Area lowerArea = Area{this->area.xMin, this->area.xMax, this->area.yMin, getMedian(points, false)};
    this->leftChild = new MyKDTree(splitVectors[0], lowerArea, lev + 1);
    Area higherArea = Area{this->area.xMin, this->area.xMax, getMedian(points, false), this->area.yMax};
    this->rightChild = new MyKDTree(splitVectors[1], higherArea, lev + 1);
}

bool MyKDTree::contains(Point point) {
    MyKDTree *current = this;
    while (!current->isLeaf()) {
        if (current->level % 2 == 0) {
            current = getMedian(current->points, true) >= point.x ? current->leftChild : current->rightChild;
        } else {
            current = getMedian(current->points, false) >= point.y ? current->leftChild : current->rightChild;
        }
    }
    return std::find(current->points.begin(), current->points.end(), point) != current->points.end();
}

list<Point> MyKDTree::query(Area &queryRectangle) {
    list<Point> result;
    if (this->isLeaf()) {
        if (containsPoint(queryRectangle, this->points[0])) {
            result.push_back(this->points[0]);
        }
        return result;
    } else if (containsArea(queryRectangle, this->area)) {
        result.insert(result.end(), this->points.begin(), this->points.end());
        return result;
    }
    if (this->leftChild != nullptr && intersects(queryRectangle, this->leftChild->area)) {
        list<Point> childResult = this->leftChild->query(queryRectangle);
        result.insert(result.end(), childResult.begin(), childResult.end());
    }
    if (this->rightChild != nullptr && intersects(queryRectangle, this->rightChild->area)) {
        list<Point> childResult = this->rightChild->query(queryRectangle);
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

bool MyKDTree::isEmpty() {
    return this->points.empty();
}

void MyKDTree::add(Point &point) {
    MyKDTree *current = this;
    int lev = 0;
    while (!current->isLeaf()) {
        if ((lev++ % 2) == 0) {
            cout << "here" << endl;
            if (point.x <= getMedian(current->points, true)) {
                current = current->leftChild;
            } else {
                current = current->rightChild;
            }
        } else {
            if (point.y <= getMedian(current->points, false)) {
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

void MyKDTree::appendPoint(Point &point, int lev) {
    points.push_back(point);
    if ((lev % 2 == 0 && points[0].x > point.x) || (lev % 2 != 0 && points[0].y > point.y)) {
        swap(points[0], point);
    }
}

void MyKDTree::kNearestNeighborsHelper(MyKDTree *node, int k,
                                       priority_queue<MyKDTree *, std::vector<MyKDTree *>, CompareMKDTree> &queue,
                                       vector<Point> &result) {
    if (node == nullptr) {
        return;
    }

    if (node->isLeaf()) {
        queue.push(node);
    } else {
        queue.push(node->leftChild);
        queue.push(node->rightChild);
    }

    while (!queue.empty() && result.size() < k) {
        MyKDTree *current = queue.top();
        queue.pop();

        if (current->isLeaf()) {
            result.push_back(current->points[0]);
        } else {
            kNearestNeighborsHelper(current, k, queue, result);
        }
    }
}

vector<Point> MyKDTree::kNearestNeighbors(Point &queryPoint, int k) {
    vector<Point> result;
    result.reserve(k);
    CompareMKDTree compareFunction(queryPoint);
    std::priority_queue<MyKDTree *, vector<MyKDTree *>, CompareMKDTree> queue(compareFunction);
    kNearestNeighborsHelper(this, k, queue, result);
    return result;
}



