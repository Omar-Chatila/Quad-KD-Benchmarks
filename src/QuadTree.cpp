//
// Created by omarc on 12/01/2024.
//

/**
 * @file QuadTree.cpp
 * @brief Implementation of a Point-QuadTree
 */

#include "../include/QuadTree.h"
#include "functional"
#include <queue>

QuadTree::QuadTree(Area square, vector<Point> &elements) {
    this->square = square;
    this->elements = elements;
}


QuadTree::~QuadTree() {
    this->elements.clear();
    for (auto &i: children) {
        delete i;
    }
}


int QuadTree::getHeight() {
    // Leaf has height one
    if (isNodeLeaf()) {
        return 1;
    }
    // Calculates heights of 4 subtrees
    int h1 = this->children[NORTH_EAST]->getHeight();
    int h2 = this->children[NORTH_WEST]->getHeight();
    int h3 = this->children[SOUTH_WEST]->getHeight();
    int h4 = this->children[SOUTH_EAST]->getHeight();
    // Height is the longest height of subtrees
    int maxHeight = max(max(h1, h2), max(h3, h4));
    return maxHeight + 1;
}


bool QuadTree::isNodeLeaf() {
    return this->children[0] == nullptr;
}


void QuadTree::buildTree() {
    // Subdivide node if it contains more than one point
    if (this->elements.size() > 1) {
        // Subdivides points and quadrants into four quadrants
        subdivide();
        // Recursively build the 4 children
        children[NORTH_EAST]->buildTree();
        children[NORTH_WEST]->buildTree();
        children[SOUTH_WEST]->buildTree();
        children[SOUTH_EAST]->buildTree();
    }
}


void QuadTree::subdivide() {
    // calculate vertical and horizontal split coordinates of the square
    double xMid = (this->square.xMin + this->square.xMax) / 2.0;
    double yMid = (this->square.yMin + this->square.yMax) / 2.0;
    // create the 4 quadrants by splitting the square
    Area *quadrants = splitArea(this->square, xMid, yMid);
    // create 4 vectors for each quadrant
    vector<Point> childrenElements[4];
    // push points of Quadtree node to the corresponding lists
    for (const auto &point: elements) {
        int quadrant = determineQuadrant(point, xMid, yMid);
        childrenElements[quadrant].push_back(point);
    }
    // Create the 4 children with the corresponding squares and elements
    for (int i = 0; i < 4; i++) {
        children[i] = new QuadTree(quadrants[i], childrenElements[i]);
    }
    free(quadrants);
}

QuadTree *QuadTree::locateQuadrant(Point &point, QuadTree *current) {
    double centerX = (current->square.xMin + current->square.xMax) / 2.0;
    double centerY = (current->square.yMin + current->square.yMax) / 2.0;
    int quadrant = determineQuadrant(point, centerX, centerY);
    return current->children[quadrant];
}


std::list<Point> QuadTree::query(Area &queryRectangle) {
    list<Point> result;
    // if Quadtree is a non-empty leaf, add its point if it is contained by queryRectangle
    if (this->isPointLeaf()) {
        if (containsPoint(queryRectangle, this->elements.front())) {
            result.push_back(this->elements.front());
        }
        return result;
        // if queryRectangle contains the Quadtree square, all its elements are inside queryRectangle
    } else if (containsArea(queryRectangle, this->square)) {
        result.insert(result.end(), this->elements.begin(), this->elements.end());
        return result;
    }
    // For each child: check if its area intersects queryRectangle. If yes, recursively call query on them
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
    // Traverses the tree from root to leaf by comparing point with coordinates of quadrants
    QuadTree *current = this;
    while (!current->isNodeLeaf()) {
        current = locateQuadrant(point, current);
    }
    // return true iff point of leaf is equal to point
    return !current->elements.empty() && current->elements[0] == point;
}


int QuadTree::determineQuadrant(const Point &point, double xMid, double yMid) {
    // Determines quadrant fast via enum indices
    int quadrant = 0b00;  // LSB: W/E. MSB: S/N
    if (point.x > xMid) {
        quadrant |= 0b01;  // East
    }
    if (point.y <= yMid) {
        quadrant |= 0b10;  // South
    }
    return quadrant;
}

bool QuadTree::isEmpty() {
    return this->elements.empty();
}


void QuadTree::add(Point &point) {
    QuadTree *current = this;
    // push point to root elements if empty
    if (current->isEmpty()) {
        current->elements.push_back(point);
        return;
    }

    // traverse to the correct location
    while (!current->isNodeLeaf()) {
        current = locateQuadrant(point, current);
    }
    current->elements.push_back(point);

    // if current has too many elements, subdivide
    if (current->elements.size() > 1) {
        current->subdivide();
    }
}

void QuadTree::kNearestNeighborsHelper(QuadTree *node, int k,
                                       std::priority_queue<QuadTree *, std::vector<QuadTree *>, CompareQuadTree> &queue,
                                       std::vector<Point> &result) {
    // Base case, abort if node is null
    if (node == nullptr) {
        return;
    }

    // in case of leaf with single node, push to queue
    if (node->isPointLeaf()) {
        queue.push(node);
    } else {
        // if node is not a leaf, push its children prioritizing proximity of their squares to queryPoint
        for (auto child: node->children) {
            if (child != nullptr) queue.push(child);
        }
    }

    // Process nodes until either queue is empty or result list has reached k elements
    while (!queue.empty() && result.size() < k) {
        QuadTree *current = queue.top();
        queue.pop();
        // push point if node is leaf containing point
        if (current->isPointLeaf()) {
            result.push_back(current->elements[0]);
            // recursively call nns-helper on highest priority non-point leaf
        } else {
            kNearestNeighborsHelper(current, k, queue, result);
        }
    }
}


std::vector<Point> QuadTree::kNearestNeighbors(Point &queryPoint, int k) {
    vector<Point> result;
    result.reserve(k);
    // comparator that prioritizes proximity of a node to the queryPoint
    CompareQuadTree compareFunction(queryPoint);
    std::priority_queue<QuadTree *, vector<QuadTree *>, CompareQuadTree> queue(compareFunction);
    // call to helper function to fill result vector with k-nearest-neighbors
    kNearestNeighborsHelper(this, k, queue, result);
    return result;
}









