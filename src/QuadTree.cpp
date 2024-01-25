//
// Created by omarc on 12/01/2024.
//

#include "../include/QuadTree.h"
#include "functional"
#include <queue>

QuadTree::QuadTree(Area square, vector<Point> &elements) {
    this->square = square;
    this->elements = elements;
}

QuadTree::~QuadTree() {
    this->elements.clear();
    for (auto & i : children) {
        delete i;
    }
}

void QuadTree::deleteTree(QuadTree *node) {
    if (node != nullptr) {
        for (auto child: node->children) {
            deleteTree(child);
        }
        delete node;
    }
}

int QuadTree::getHeight() {
    if (isNodeLeaf()) {
        return 1;
    }
    int h1 = this->children[NORTH_EAST]->getHeight();
    int h2 = this->children[NORTH_WEST]->getHeight();
    int h3 = this->children[SOUTH_WEST]->getHeight();
    int h4 = this->children[SOUTH_EAST]->getHeight();
    int maxHeight = max(max(h1, h2), max(h3, h4));
    return maxHeight + 1;
}

bool QuadTree::isNodeLeaf() {
    return this->children[0] == nullptr;
}

void QuadTree::buildTree() {
    if (this->elements.size() > 1) {
        partition();
        children[NORTH_EAST]->buildTree();
        children[NORTH_WEST]->buildTree();
        children[SOUTH_WEST]->buildTree();
        children[SOUTH_EAST]->buildTree();
    }
}

void QuadTree::partition() {
    double xMid = (this->square.xMin + this->square.xMax) / 2.0;
    double yMid = (this->square.yMin + this->square.yMax) / 2.0;
    Area *quadrants = splitArea(this->square, xMid, yMid);
    vector<Point> childrenElements[4];

    for (auto point: elements) {
        int quadrant = 0B00;           // lsb W/E msb msb S/N
        if (point.x > xMid) {          // is East
            quadrant |= 0B01;
        }

        if (point.y <= yMid) {         // is South
            quadrant |= 0B10;
        }
        childrenElements[quadrant].push_back(point);
    }
    //elements.clear();

    for (int i = 0; i < 4; i++) {
        children[i] = new QuadTree(quadrants[i], childrenElements[i]);
    }
    free(quadrants);
}

std::list<Point> QuadTree::query(Area &queryRectangle) {
    list<Point> result;
    if (this->isPointLeaf()) {
        if (containsPoint(queryRectangle, this->elements.front())) {
            result.push_back(this->elements.front());
        }
        return result;
    } else if (containsArea(queryRectangle, this->square)) {
        result.insert(result.end(), this->elements.begin(), this->elements.end());
        return result;
    }

    for (auto child: this->children) {
        if (child != nullptr && intersects(queryRectangle, child->square)) {
            list<Point> childResult = child->query(queryRectangle);
            childResult.insert(result.end(), childResult.begin(), childResult.end());
        }
    }
    return result;
}

bool QuadTree::isPointLeaf() {
    return this->elements.size() == 1;
}

bool QuadTree::contains(Point &point) {
    QuadTree *current = this;
    while (!current->isNodeLeaf()) {
        current = locateQuadrant(point.x, point.y, current);
    }
    return !current->elements.empty() && current->elements[0] == point;
}

QuadTree *QuadTree::locateQuadrant(double pointX, double pointY, QuadTree *current) {
    double centerX = (current->square.xMin + current->square.xMax) / 2.0;
    double centerY = (current->square.yMin + current->square.yMax) / 2.0;

    int quadrant = 0B00;           // lsb W/E msb msb S/N
    if (pointX > centerX) {          // is East
        quadrant |= 0B01;
    }
    if (pointY <= centerY) {         // is South
        quadrant |= 0B10;
    }

    return current->children[quadrant];
}

bool QuadTree::isEmpty() {
    return this->elements.empty();
}

void QuadTree::add(Point &point) {
    QuadTree *current = this;
    if (current->isEmpty()) {
        current->elements.push_back(point);
        return;
    }

    while (!current->isNodeLeaf()) {
        current = locateQuadrant(point.x, point.y, current);
    }
    current->elements.push_back(point);

    if (current->elements.size() > 1) {
        current->partition();
    }
}

QuadTree *QuadTree::getNorthEast() {
    return this->children[NORTH_EAST];
}

QuadTree *QuadTree::getNorthWest() {
    return this->children[NORTH_WEST];
}

QuadTree *QuadTree::getSouthWest() {
    return this->children[SOUTH_WEST];
}

QuadTree *QuadTree::getSouthEast() {
    return this->children[SOUTH_EAST];
}

void QuadTree::kNearestNeighborsHelper(QuadTree *node, int k,
                                       std::priority_queue<QuadTree *, std::vector<QuadTree *>, CompareQuadTree> &queue,
                                       std::vector<Point> &result) {
    if (node == nullptr) {
        return;
    }

    if (node->isPointLeaf()) {
        queue.push(node);
    } else {
        for (auto child: node->children) {
            if (child != nullptr)
                queue.push(child);
        }
    }

    while (!queue.empty() && result.size() < k) {
        QuadTree *current = queue.top();
        queue.pop();

        if (current->isPointLeaf()) {
            result.push_back(current->elements[0]);
        } else {
            kNearestNeighborsHelper(current, k, queue, result);
        }
    }
}

std::vector<Point> QuadTree::kNearestNeighbors(Point &queryPoint, int k) {
    vector<Point> result;
    result.reserve(k);
    CompareQuadTree compareFunction(queryPoint);
    std::priority_queue<QuadTree *, vector<QuadTree *>, CompareQuadTree> queue(compareFunction);
    kNearestNeighborsHelper(this, k, queue, result);
    return result;
}









