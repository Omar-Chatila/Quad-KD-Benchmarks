//
// Created by omarc on 12/01/2024.
//

#include "../include/KDTreeEfficient.h"

KDTreeEfficient::KDTreeEfficient(Point *points, int level, Area &area, int from, int to) {
    this->points = points;
    this->area = area;
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

KDTreeEfficient::~KDTreeEfficient() {
    free(this->points);
}


void KDTreeEfficient::setVerticalChildren(int level) {
    int midIndex = (from + to) / 2;
    Area leftArea = Area{this->area.xMin, this->xMedian, this->area.yMin, this->area.yMax};
    Area rightArea = Area{this->xMedian, this->area.xMax, this->area.yMin, this->area.yMax};

    this->leftChild = new KDTreeEfficient(this->points, level + 1, leftArea, from, midIndex);
    this->rightChild = new KDTreeEfficient(this->points, level + 1, rightArea, midIndex + 1, to);
}

void KDTreeEfficient::setHorizontalChildren(int level) {
    int midIndex = (from + to) / 2;
    Area lowerArea = Area{this->area.xMin, this->area.xMax, this->area.yMin, this->yMedian};
    Area higherArea = Area{this->area.xMin, this->area.xMax, this->yMedian, this->area.yMax};
    this->leftChild = new KDTreeEfficient(this->points, level + 1, lowerArea, from, midIndex);
    this->rightChild = new KDTreeEfficient(this->points, level + 1, higherArea, midIndex + 1, to);
}

void KDTreeEfficient::buildTree() {
    buildTree(0);
}

void KDTreeEfficient::buildTree(int level) {
    if (this->to - this->from > 0) {
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

bool KDTreeEfficient::contains(Point point) {
    double pointX = point.x;
    double pointY = point.y;
    KDTreeEfficient *current = this;
    int level = 0;
    while (!current->isLeaf()) {
        if (level % 2 == 0) {
            current = current->xMedian >= pointX ? current->leftChild : current->rightChild;
        } else {
            current = current->yMedian >= pointY ? current->leftChild : current->rightChild;
        }
        level++;
    }
    return current->points[current->from] == point;
}

bool KDTreeEfficient::isLeaf() {
    return this->from == this->to;
}

int KDTreeEfficient::getHeight() {
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

std::list<Point> KDTreeEfficient::query(Area queryRectangle) {
    list<Point> result;
    if (this->isLeaf()) {
        if (containsPoint(queryRectangle, this->points[from])) {
            result.push_back(this->points[from]);
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

KDTreeEfficient *KDTreeEfficient::getLeftChild() {
    return this->leftChild;
}

KDTreeEfficient *KDTreeEfficient::getRightChild() {
    return this->rightChild;
}

Point *KDTreeEfficient::getPoints() {
    return this->points;
}

vector<Point> KDTreeEfficient::kNearestNeighbors(Point &queryPoint, int k) {
    vector<Point> result;
    result.reserve(k);
    CompareKDTree compareFunction(queryPoint);
    std::priority_queue<KDTreeEfficient *, vector<KDTreeEfficient *>, CompareKDTree> queue(compareFunction);
    kNearestNeighborsHelper(this, k, queue, result);
    return result;
}

void KDTreeEfficient::kNearestNeighborsHelper(KDTreeEfficient *node, int k,
                                              priority_queue<KDTreeEfficient *, std::vector<KDTreeEfficient *>, CompareKDTree> &queue,
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
        KDTreeEfficient *current = queue.top();
        queue.pop();

        if (current->isLeaf()) {
            result.push_back(current->points[from]);
        } else {
            kNearestNeighborsHelper(current, k, queue, result);
        }
    }
}








