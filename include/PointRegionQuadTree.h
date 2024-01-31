//
// Created by omarc on 12/01/2024.
//

#ifndef QUADKDBENCH_PointRegionQuadTree_H
#define QUADKDBENCH_PointRegionQuadTree_H

#include "Util.h"
#include <bits/stdc++.h>

/**
 * @brief A class representing a Point-Region-QuadTree data structure.
 */
class PointRegionQuadTree {
    /**
     * @brief A comparison struct for prioritizing Point-Region-QuadTree nodes based on distance from a query point.
     */
    struct ComparePointRegionQuadTree {
        const Point &queryPoint;

        /**
         * @brief Constructor for CompareQuadTree.
         * @param p The query point to calculate distances from.
         */
        explicit ComparePointRegionQuadTree(const Point &p) : queryPoint(p) {}

        /**
         * @brief Overloaded operator to compare QuadTree nodes based on squared distance from the query point.
         * @param a The first QuadTree node.
         * @param b The second QuadTree node.
         * @return True if the squared distance from node a is greater than from node b; false otherwise.
         */
        bool operator()(PointRegionQuadTree *a, PointRegionQuadTree *b) const {
            return sqDistanceFrom(a->square, queryPoint) > sqDistanceFrom(b->square, queryPoint);
        }
    };

    /**
     * Overloads output stream operator to print information of KD-Tree node
     * @param os
     * @param kdTreeEfficient
     * @return String representation of KD-Tree
     */
    friend std::ostream &operator<<(std::ostream &os, const PointRegionQuadTree &PointRegionQuadTree) {
        os << std::fixed << std::setprecision(1);
        if (!PointRegionQuadTree.elements.empty())
            return os << "A:" << PointRegionQuadTree.square << "elements:" << PointRegionQuadTree.elements.front()
                      << "\n";
        return os << "A:" << PointRegionQuadTree.square << "\n";
    }

private:
    PointRegionQuadTree *children[4]{};  /**< Pointers to the 4 children of the QuadTree node. */
    Area square{};                       /**< The area covered by the QuadTree node. */
    vector<Point> elements;              /**< The vector of points associated with the QuadTree node. */
    int capacity;                        /**< Capacity of a leaf */

    /**
    * @brief Locates the quadrant of the QuadTree based on the specified coordinates.
    * @param pointX The x-coordinate of the point to be located.
    * @param pointY The y-coordinate of the point to be located.
    * @param current The current QuadTree node being considered.
    * @return The QuadTree node representing the located quadrant.
    */
    static PointRegionQuadTree *locateQuadrant(double pointX, double pointY, PointRegionQuadTree *current);

    /**
    * @brief Subdivides the square and points into 4 partitions and creates 4 children
    */
    void subdivide();

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
    void kNearestNeighborsHelper(PointRegionQuadTree *node, int k,
                                 priority_queue<PointRegionQuadTree *, std::vector<PointRegionQuadTree *>, ComparePointRegionQuadTree> &queue,
                                 std::vector<Point> &result, Point &queryPoint);

public:
    /**
    * @brief Constructs a QuadTree with the specified square area and elements.
    * @param square The square area covered by the QuadTree.
    * @param elements The vector of points contained in the QuadTree.
    */
    PointRegionQuadTree(Area square, vector<Point> &elements, int capacity);

    /**
    * @brief destroys the Quadtree and deallocates memory
    */
    ~PointRegionQuadTree();

    /**
    * @brief Checks if a node is a leaf
    * @return True if node is leaf, false otherwise
    */
    bool isNodeLeaf();

    /**
    * @brief Checks if a node is a leaf containing a point
    * @return True if node is a leaf containing a point, false otherwise
    */
    bool isPointLeaf();

    /**
    * @brief Calculates height of the Quadtree
    * @return The height of the Quadtree
    */
    int getHeight();

    /**
    * @brief Builds the Quadtree
    * Builds Quadtree by subdividing its square and points into 4 Quadrants. Splits as long as elements.size() > 1
    */
    void buildTree();

    /**
    * @param queryRectangle Rectangle that contains points of interest
    * @return list<Point> of points contained by queryRectangle
    */
    list<Point> query(Area &queryRectangle);

    /**
    * @brief Checks if a given point is contained by the Quadtree
    * @param point
    * @return True if Quadtree contains point, false otherwise
    */
    bool contains(Point &point);

    /**
    * Checks if Quadtree is empty, meaning it does not contains any elements
    * @return True if Quadtree is empty, false otherwise
    */
    bool isEmpty();

    /**
     * Adds a given Point to the Quadtree
     * @param point Point to be added
     */
    void add(Point &point);

    /**
     * Get k nearest neighbors of a query point
     * @param queryPoint The point of which the k nearest neighbors are determined
     * @param k The number of neighbors
     * @return vector containing k nearest neighbors of queryPoint
     */
    vector<Point> kNearestNeighbors(Point &queryPoint, int k);
};


#endif //QUADKDBENCH_PointRegionQuadTree_H
