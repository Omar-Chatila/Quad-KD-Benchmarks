/**
 * @author Omar Chatila
 * @file QuadTree.h
 * @brief Implementation of a Point-QuadTree with leaf capacity 1
 */


#ifndef QUADKDBENCH_QUADTREE_H
#define QUADKDBENCH_QUADTREE_H

#include "Util.h"
#include <bits/stdc++.h>

/**
 * @brief A class representing a QuadTree data structure.
 */
class QuadTree {
    /**
     * @brief A comparison struct for prioritizing QuadTree nodes based on distance from a query point.
     */
    struct CompareQuadTree {
        const Point &queryPoint;

        /**
         * @brief Constructor for CompareQuadTree.
         * @param p The query point to calculate distances from.
         */
        explicit CompareQuadTree(const Point &p) : queryPoint(p) {}

        /**
         * @brief Overloaded operator to compare QuadTree nodes based on squared distance from the query point.
         * @param a The first QuadTree node.
         * @param b The second QuadTree node.
         * @return True if the squared distance from node a is greater than from node b; false otherwise.
         */
        bool operator()(QuadTree *a, QuadTree *b) const {
            return sqDistanceFrom(a->square, queryPoint) > sqDistanceFrom(b->square, queryPoint);
        }
    };

    /**
     * @brief Overloaded << operator to stream the QuadTree information.
     * @param os The output stream.
     * @param quadTree The QuadTree instance to be streamed.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const QuadTree &quadTree) {
        os << std::fixed << std::setprecision(1);
        if (!quadTree.elements.empty())
            return os << "A:" << quadTree.square << "elements:" << quadTree.elements.front() << "\n";
        return os << "A:" << quadTree.square << "\n";
    }

private:
    QuadTree *children[4]{}; /**< Pointers to the 4 children of the QuadTree node. */
    Area square;             /**< The area covered by the QuadTree node. */
    vector<Point> elements;  /**< The vector of points associated with the QuadTree node. */

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
    void kNearestNeighborsHelper(QuadTree *node, int k,
                                 priority_queue<QuadTree *, std::vector<QuadTree *>, CompareQuadTree> &queue,
                                 std::vector<Point> &result);

    /**
     * @brief Determines the quadrant of a point based on the given split coordinates.
     * @param point The point to be located.
     * @param xMid The x-coordinate of the midpoint.
     * @param yMid The y-coordinate of the midpoint.
     * @return The quadrant index (0 to 3) where the point belongs.
     */
    static int determineQuadrant(const Point &point, double xMid, double yMid);

    /**
     * @brief Locates the quadrant of the QuadTree based on the specified coordinates.
     * @param pointX The x-coordinate of the point to be located.
     * @param pointY The y-coordinate of the point to be located.
     * @param current The current QuadTree node being considered.
     * @return The QuadTree node representing the located quadrant.
     */
    static QuadTree *locateQuadrant(Point &point, QuadTree *current);

    /**
     * @brief Subdivides the square and points into 4 partitions and creates 4 children
     */
    void subdivide();

public:
    /**
     * @brief Constructs a QuadTree with the specified square area and elements.
     * @param square The square area covered by the QuadTree.
     * @param elements The vector of points contained in the QuadTree.
     */
    QuadTree(Area square, vector<Point> &elements);

    /**
     * @brief destroys the Quadtree and deallocates memory
     */
    ~QuadTree();

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


#endif //QUADKDBENCH_QUADTREE_H
