//
// Created by omarc on 12/01/2024.
//

#include "../include/QuadTree.h"

QuadTree::QuadTree(Area square, vector<Point> &elements) {
    this->square = square;
    this->elements.insert(this->elements.end(), elements.begin(), elements.end());
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
    elements.clear();

    for (int i = 0; i < 4; i++) {
        children[i] = new QuadTree(quadrants[i], childrenElements[i]);
    }
}

vector<Point> QuadTree::query(Area queryRectangle) {
    if (!intersects(this->square, queryRectangle)) {
        return {};
    }
    vector<Point> result;
    if (this->isPointLeaf()) {
        if (containsPoint(queryRectangle, this->elements.at(0))) {
            result.push_back(this->elements.at(0));
        }
    } else if (containsArea(queryRectangle, this->square)) {
        result.insert(result.end(), this->elements.begin(), this->elements.end());
    }

    for (auto child: this->children) {
        if (child != nullptr && intersects(queryRectangle, child->square)) {
            vector<Point> childResult = child->query(queryRectangle);
            childResult.insert(result.end(), childResult.begin(), childResult.end());
        }
    }
    return result;
}

bool QuadTree::isPointLeaf() {
    return !this->elements.empty();
}

bool QuadTree::contains(Point point) {
    QuadTree *current = this;
    while (!current->isNodeLeaf()) {
        current = locateQuadrant(point.x, point.y, current);
    }
    auto it = find(current->elements.begin(), current->elements.end(), point);
    return it != current->elements.end();
}

QuadTree *QuadTree::locateQuadrant(double pointX, double pointY, QuadTree *current) {
    double centerX = (current->square.xMin + current->square.xMax) / 2.0;
    double centerY = (current->square.yMin + current->square.yMax) / 2.0;
    if (pointX > centerX && pointY > centerY) {
        current = current->children[NORTH_EAST];
    } else if (pointX <= centerX && pointY > centerY) {
        current = current->children[NORTH_WEST];
    } else if (pointX <= centerX && pointY <= centerY) {
        current = current->children[SOUTH_WEST];
    } else {
        current = current->children[SOUTH_EAST];
    }
    return current;
}

bool QuadTree::isEmpty() {
    return this->elements.empty();
}

void QuadTree::add(Point point) {
    QuadTree *current = this;
    if (!current->isEmpty()) {
        double pointX = point.x;
        double pointY = point.y;
        while (!current->isNodeLeaf()) {
            current = locateQuadrant(pointX, pointY, current);
        }
        current->elements.push_back(point);
        if (current->isPointLeaf()) {
            current->buildTree();
        }
    } else {
        current->elements.push_back(point);
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




