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

/**
 * @brief Constructs a QuadTree with the specified square area and elements.
 * @param square The square area covered by the QuadTree.
 * @param elements The vector of points contained in the QuadTree.
 */
QuadTree::QuadTree(Area square, vector<Point> &elements) {
    this->square = square;
    this->elements = elements;
}

/**
 * @brief destroys the Quadtree and deallocates memory
 */
QuadTree::~QuadTree() {
    this->elements.clear();
    for (auto &i: children) {
        delete i;
    }
}

/**
 * @brief Calculates height of the Quadtree
 * @return The height of the Quadtree
 */
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

/**
 * @brief Checks if a node is a leaf
 * @return True if node is leaf, false otherwise
 */
bool QuadTree::isNodeLeaf() {
    return this->children[0] == nullptr;
}

/**
 * @brief Builds the Quadtree by subdividing its square and points into 4 Quadrants. Splits as long as elements.size() > 1
 */
void QuadTree::buildTree() {
    // Subdivide node if it contains more than one point
    if (this->elements.size() > 1) {
        // Subdivides points and quadrants into four quadrants
        partition();
        // Recursively build the 4 children
        children[NORTH_EAST]->buildTree();
        children[NORTH_WEST]->buildTree();
        children[SOUTH_WEST]->buildTree();
        children[SOUTH_EAST]->buildTree();
    }
}

/**
 * @brief Subdivides the square and points into 4 partitions and creates 4 children
 */
void QuadTree::partition() {
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

/**
 * @brief Locates the quadrant of the QuadTree based on the specified coordinates.
 * @param pointX The x-coordinate of the point to be located.
 * @param pointY The y-coordinate of the point to be located.
 * @param current The current QuadTree node being considered.
 * @return The QuadTree node representing the located quadrant.
 */
QuadTree *QuadTree::locateQuadrant(Point &point, QuadTree *current) {
    double centerX = (current->square.xMin + current->square.xMax) / 2.0;
    double centerY = (current->square.yMin + current->square.yMax) / 2.0;
    int quadrant = determineQuadrant(point, centerX, centerY);
    return current->children[quadrant];
}

/**
 * @param queryRectangle Rectangle that contains points of interest
 * @return list<Point> of points contained by queryRectangle
 */
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

/**
 * @brief Checks if a node is a leaf containing a point
 * @return True if node is a leaf containing a point, false otherwise
 */
bool QuadTree::isPointLeaf() {
    return this->elements.size() == 1;
}

/**
 * @brief Checks if a given point is contained by the Quadtree
 * @param point
 * @return True if Quadtree contains point, false otherwise
 */
bool QuadTree::contains(Point &point) {
    // Traverses the tree from root to leaf by comparing point with coordinates of quadrants
    QuadTree *current = this;
    while (!current->isNodeLeaf()) {
        current = locateQuadrant(point, current);
    }
    // return true iff point of leaf is equal to point
    return !current->elements.empty() && current->elements[0] == point;
}

/**
 * @brief Determines the quadrant of a point based on the given split coordinates.
 * @param point The point to be located.
 * @param xMid The x-coordinate of the midpoint.
 * @param yMid The y-coordinate of the midpoint.
 * @return The quadrant index (0 to 3) where the point belongs.
 */
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

/**
 * Checks if Quadtree is empty, meaning it does not contains any elements
 * @return True if Quadtree is empty, false otherwise
 */
bool QuadTree::isEmpty() {
    return this->elements.empty();
}

/**
 * Adds a given Point to the Quadtree
 * @param point Point to be added
 */
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
        current->partition();
    }
}

/**
 * Gets the child node representing the north-east quadrant of the Quadtree
 * @return north-east child
 */
QuadTree *QuadTree::getNorthEast() {
    return this->children[NORTH_EAST];
}

/**
 * Gets the child node representing the north-west quadrant of the Quadtree
 * @return north-west child
 */
QuadTree *QuadTree::getNorthWest() {
    return this->children[NORTH_WEST];
}

/**
 * Gets the child node representing the south-west quadrant of the Quadtree
 * @return south-west child
 */
QuadTree *QuadTree::getSouthWest() {
    return this->children[SOUTH_WEST];
}

/**
 * Gets the child node representing the south-east quadrant of the Quadtree
 * @return south-east child
 */
QuadTree *QuadTree::getSouthEast() {
    return this->children[SOUTH_EAST];
}

/**
 * @brief Helper method for kNearestNeighbors(Point &queryPoint, int k). Adds k-nearest neighbor of Quadtree node to result vector
 *
 * Recursively traverses Quadtree nodes to find k nearest neighbors. Uses priority to keep track of proximity to query Point
 *
 * @param node The Quadtree that is used for k-NNS
 * @param k The number of neighbors
 * @param queue Priority Queue that prioritizes nodes by proximity of its square to a queryPoint
 * @param result Vector that contains the k nearest neighbors
 */
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

/**
 * Get k nearest neighbors of a query point
 * @param queryPoint The point of which the k nearest neighbors are determined
 * @param k The number of neighbors
 * @return vector containing k nearest neighbors of queryPoint
 */
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









