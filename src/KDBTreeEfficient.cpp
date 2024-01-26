//
// Created by omarc on 12/01/2024.
//

#include "../include/KDBTreeEfficient.h"

KDBTreeEfficient::KDBTreeEfficient(Point *points, int level, Area &area, int from, int to, int capacity) {
    this->points = points;
    this->area = area;
    this->capacity = capacity;
    this->from = from;
    this->to = to;
    if (level % 2 == 0) {
        this->xMedian = median(points, true, from, to);
        this->yMedian = 0.0;
    } else {
        this->yMedian = median(points, false, from, to);
        this->xMedian = 0.0;
    }
}

KDBTreeEfficient::~KDBTreeEfficient() {
    delete leftChild;
    delete rightChild;
}

void KDBTreeEfficient::setVerticalChildren(int level) {
    int midIndex = (from + to) / 2;
    Area leftArea = Area{this->area.xMin, this->xMedian, this->area.yMin, this->area.yMax};
    Area rightArea = Area{this->xMedian, this->area.xMax, this->area.yMin, this->area.yMax};

    this->leftChild = new KDBTreeEfficient(this->points, level + 1, leftArea, from, midIndex, capacity);
    this->rightChild = new KDBTreeEfficient(this->points, level + 1, rightArea, midIndex + 1, to, capacity);
}

void KDBTreeEfficient::setHorizontalChildren(int level) {
    int midIndex = (from + to) / 2;
    Area lowerArea = Area{this->area.xMin, this->area.xMax, this->area.yMin, this->yMedian};
    Area higherArea = Area{this->area.xMin, this->area.xMax, this->yMedian, this->area.yMax};
    this->leftChild = new KDBTreeEfficient(this->points, level + 1, lowerArea, from, midIndex, capacity);
    this->rightChild = new KDBTreeEfficient(this->points, level + 1, higherArea, midIndex + 1, to, capacity);
}

void KDBTreeEfficient::buildTree() {
    buildTree(0);
}

void KDBTreeEfficient::buildTree(int level) {
    if (this->to - this->from >= capacity) {
        if (level % 2 == 0) {
            // vertical split
            this->setVerticalChildren(level);
        } else {
            // horizontal split
            this->setHorizontalChildren(level);
        }
        this->leftChild->buildTree(level + 1);
        this->rightChild->buildTree(level + 1);
    }
}

bool KDBTreeEfficient::contains(Point point) {
    double pointX = point.x;
    double pointY = point.y;
    KDBTreeEfficient *current = this;
    int level = 0;
    while (!current->isLeaf()) {
        if (level % 2 == 0) {
            current = current->xMedian >= pointX ? current->leftChild : current->rightChild;
        } else {
            current = current->yMedian >= pointY ? current->leftChild : current->rightChild;
        }
        level++;
    }
    for (int i = 0; i < capacity; i++) {
        if (current->points[i] == point) return true;
    }
    return false;
}

bool KDBTreeEfficient::isLeaf() const {
    return this->to - this->from < this->capacity;
}

int KDBTreeEfficient::getHeight() {
    if (this->isLeaf()) {
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

std::list<Point> KDBTreeEfficient::query(Area queryRectangle) {
    list<Point> result;
    if (this->isLeaf()) {
        if (containsPoint(queryRectangle, this->points[from])) {
            for (int i = this->from; i <= this->to; i++) {
                if (containsPoint(queryRectangle, this->points[i])) {
                    result.push_back(this->points[i]);
                }
            }
        }
        return result;
    } else if (containsArea(queryRectangle, this->area)) {
        result.insert(result.end(), this->points + from, this->points + to + 1);
        return result;
    }

    if (this->leftChild != nullptr && intersects(queryRectangle, this->leftChild->area)) {
        list<Point> childResult = this->leftChild->query(queryRectangle);
        childResult.insert(result.end(), childResult.begin(), childResult.end());
    }
    if (this->rightChild != nullptr && intersects(queryRectangle, this->rightChild->area)) {
        list<Point> childResult = this->rightChild->query(queryRectangle);
        childResult.insert(result.end(), childResult.begin(), childResult.end());
    }
    return result;
}

KDBTreeEfficient *KDBTreeEfficient::getLeftChild() {
    return this->leftChild;
}

KDBTreeEfficient *KDBTreeEfficient::getRightChild() {
    return this->rightChild;
}

Point *KDBTreeEfficient::getPoints() {
    return this->points;
}

vector<Point> KDBTreeEfficient::kNearestNeighbors(Point &queryPoint, int k) {
    vector<Point> result;
    result.reserve(k);
    CompareKDBTree compareFunction(queryPoint);
    std::priority_queue<KDBTreeEfficient *, vector<KDBTreeEfficient *>, CompareKDBTree> queue(compareFunction);
    kNearestNeighborsHelper(this, k, queue, result, queryPoint);
    return result;
}

void KDBTreeEfficient::kNearestNeighborsHelper(KDBTreeEfficient *node, int k,
                                               priority_queue<KDBTreeEfficient *, std::vector<KDBTreeEfficient *>, CompareKDBTree> &queue,
                                               vector<Point> &result, Point &queryPoint) {
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
        KDBTreeEfficient *current = queue.top();
        queue.pop();
        if (current->isLeaf()) {
            std::sort(current->points + current->from, current->points + current->to,
                      [queryPoint](const Point &A, const Point &B) {
                          return pointDistance(queryPoint, A) < pointDistance(queryPoint, B);
                      });
            for (int i = current->from; i <= current->to; i++) {
                if (result.size() < k) {
                    result.push_back(current->points[i]);
                }
            }
        } else {
            kNearestNeighborsHelper(current, k, queue, result, queryPoint);
        }
    }
}










