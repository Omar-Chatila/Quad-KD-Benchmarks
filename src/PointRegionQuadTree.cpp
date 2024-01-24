//
// Created by omarc on 12/01/2024.
//

#include "../include/PointRegionQuadTree.h"
#include "functional"
#include <queue>

PointRegionQuadTree::PointRegionQuadTree(Area square, vector<Point> &elements, int capacity) {
    this->square = square;
    this->elements = elements;
    this->capacity = capacity;
}

int PointRegionQuadTree::getHeight() {
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

bool PointRegionQuadTree::isNodeLeaf() {
    return this->children[0] == nullptr;
}

void PointRegionQuadTree::buildTree() {
    if (this->elements.size() > capacity) {
        partition();
        children[NORTH_EAST]->buildTree();
        children[NORTH_WEST]->buildTree();
        children[SOUTH_WEST]->buildTree();
        children[SOUTH_EAST]->buildTree();
    }
}

void PointRegionQuadTree::partition() {
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
        children[i] = new PointRegionQuadTree(quadrants[i], childrenElements[i], capacity);
    }
    free(quadrants);
}

std::list<Point> PointRegionQuadTree::query(Area &queryRectangle) {
    list<Point> result;
    if (this->isPointLeaf()) {
        for (auto point : this->elements) {
            if (containsPoint(queryRectangle, point)) {
                result.push_back(point);
            }
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

bool PointRegionQuadTree::isPointLeaf() {
    return !this->elements.empty() && this->elements.size() <= capacity;
}

bool PointRegionQuadTree::contains(Point &point) {
    PointRegionQuadTree *current = this;
    while (!current->isNodeLeaf()) {
        current = locateQuadrant(point.x, point.y, current);
    }
    return !current->elements.empty()
        && find(current->elements.begin(), current->elements.end(), point) != current->elements.end();
}

PointRegionQuadTree *PointRegionQuadTree::locateQuadrant(double pointX, double pointY, PointRegionQuadTree *current) {
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

bool PointRegionQuadTree::isEmpty() {
    return this->elements.empty();
}

void PointRegionQuadTree::add(Point &point) {
    PointRegionQuadTree *current = this;
    if (current->isEmpty()) {
        current->elements.push_back(point);
        return;
    }

    while (!current->isNodeLeaf()) {
        current = locateQuadrant(point.x, point.y, current);
    }
    current->elements.push_back(point);

    if (current->elements.size() > capacity) {
        current->partition();
    }
}

PointRegionQuadTree *PointRegionQuadTree::getNorthEast() {
    return this->children[NORTH_EAST];
}

PointRegionQuadTree *PointRegionQuadTree::getNorthWest() {
    return this->children[NORTH_WEST];
}

PointRegionQuadTree *PointRegionQuadTree::getSouthWest() {
    return this->children[SOUTH_WEST];
}

PointRegionQuadTree *PointRegionQuadTree::getSouthEast() {
    return this->children[SOUTH_EAST];
}

void PointRegionQuadTree::kNearestNeighborsHelper(PointRegionQuadTree *node, int k,
                                       std::priority_queue<PointRegionQuadTree *, std::vector<PointRegionQuadTree *>, ComparePointRegionQuadTree> &queue,
                                       std::vector<Point> &result, Point &queryPoint) {
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
        PointRegionQuadTree *current = queue.top();
        queue.pop();
        if (current->isPointLeaf()) {
            std::sort(current->elements.begin(), current->elements.end(),[queryPoint] (const Point &A, const Point &B){
                return pointDistance(queryPoint, A) < pointDistance(queryPoint, B);
            });
            for (auto leafPoint : current->elements) {
                if (result.size() < k) {
                    result.push_back(leafPoint);
                }
            }
        } else {
            kNearestNeighborsHelper(current, k, queue, result, queryPoint);
        }
    }
}

std::vector<Point> PointRegionQuadTree::kNearestNeighbors(Point &queryPoint, int k) {
    vector<Point> result;
    result.reserve(k);
    ComparePointRegionQuadTree compareFunction(queryPoint);
    std::priority_queue<PointRegionQuadTree *, vector<PointRegionQuadTree *>, ComparePointRegionQuadTree> queue(compareFunction);
    kNearestNeighborsHelper(this, k, queue, result, queryPoint);
    return result;
}







