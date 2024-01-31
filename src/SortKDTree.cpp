//
// Created by omarc on 12/01/2024.
//

#include "../include/SortKDTree.h"

SortKDTree::SortKDTree(vector<Point> &points, Area &area, int level) {
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


SortKDTree::~SortKDTree() {
    this->points.clear();
    delete this->leftChild;
    delete this->rightChild;
}


void SortKDTree::buildTree(int lev) {
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

void SortKDTree::buildTree() {
    buildTree(0);
}

void SortKDTree::setVerticalChildren(int lev) {
    std::vector<std::vector<Point>> splitVectors = splitVector(points);
    Area leftArea = Area{this->area.xMin, getMedian(points, true), this->area.yMin, this->area.yMax};
    this->leftChild = new SortKDTree(splitVectors[0], leftArea, lev + 1);
    Area rightArea = Area{getMedian(points, true), this->area.xMax, this->area.yMin, this->area.yMax};
    this->rightChild = new SortKDTree(splitVectors[1], rightArea, lev + 1);
}

void SortKDTree::setHorizontalChildren(int lev) {
    std::vector<std::vector<Point>> splitVectors = splitVector(points);
    Area lowerArea = Area{this->area.xMin, this->area.xMax, this->area.yMin, getMedian(points, false)};
    this->leftChild = new SortKDTree(splitVectors[0], lowerArea, lev + 1);
    Area higherArea = Area{this->area.xMin, this->area.xMax, getMedian(points, false), this->area.yMax};
    this->rightChild = new SortKDTree(splitVectors[1], higherArea, lev + 1);
}

bool SortKDTree::contains(Point point) {
    SortKDTree *current = this;
    while (!current->isLeaf()) {
        if (current->level % 2 == 0) {
            current = getMedian(current->points, true) >= point.x ? current->leftChild : current->rightChild;
        } else {
            current = getMedian(current->points, false) >= point.y ? current->leftChild : current->rightChild;
        }
    }
    return std::find(current->points.begin(), current->points.end(), point) != current->points.end();
}

list<Point> SortKDTree::query(Area &queryRectangle) {
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

int SortKDTree::getHeight() {
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

bool SortKDTree::isLeaf() {
    return this->points.size() == 1;
}

void SortKDTree::add(Point &point) {
    SortKDTree *current = this;
    int lev = 0;
    while (!current->isLeaf()) {
        if ((lev++ % 2) == 0) {
            // check X-coordinate
            if (point.x <= (current->area.xMin + current->area.xMax) / 2.0) {
                current = current->leftChild;
            } else {
                current = current->rightChild;
            }
            // check y-coordinate
        } else {
            if (point.y <= (current->area.yMin + current->area.yMax) / 2.0) {
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

void SortKDTree::appendPoint(Point &point, int lev) {
    points.push_back(point);
    if ((lev % 2 == 0 && points[0].x > point.x) || (lev % 2 != 0 && points[0].y > point.y)) {
        swap(points[0], point);
    }
}

void SortKDTree::kNearestNeighborsHelper(SortKDTree *node, int k,
                                         priority_queue<SortKDTree *, std::vector<SortKDTree *>, CompareMKDTree> &queue,
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
        SortKDTree *current = queue.top();
        queue.pop();

        if (current->isLeaf()) {
            result.push_back(current->points[0]);
        } else {
            kNearestNeighborsHelper(current, k, queue, result);
        }
    }
}

vector<Point> SortKDTree::kNearestNeighbors(Point &queryPoint, int k) {
    vector<Point> result;
    result.reserve(k);
    CompareMKDTree compareFunction(queryPoint);
    std::priority_queue<SortKDTree *, vector<SortKDTree *>, CompareMKDTree> queue(compareFunction);
    kNearestNeighborsHelper(this, k, queue, result);
    return result;
}



