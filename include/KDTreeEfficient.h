/**
 * @author Omar Chatila
 * @file KDTreeEfficient.cpp
 * @brief Implementation of a KD-Tree
 *
 * This version of KD-Trees uses a linear medium function
 * Building the tree in O(nlog(n))
 */

#ifndef QUADKDBENCH_KDTREEEFFICIENT_H
#define QUADKDBENCH_KDTREEEFFICIENT_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class KDTreeEfficient {
    /**
     * @brief A comparison struct for prioritizing KD-Tree areas based on distance from a query point.
     */
    struct CompareKDETree {
        const Point &queryPoint;

        /**
         * @brief Constructor for CompareMKDTree.
         * @param p The query point to calculate distances from.
         */
        explicit CompareKDETree(const Point &p) : queryPoint(p) {}

        /**
         * @brief Overloaded operator to compare KD-Tree nodes based on squared distance from the query point.
         * @param a The first KD-Tree node.
         * @param b The second KD-Tree node.
         * @return True if the squared distance of point p to  node a is greater than to node b; false otherwise.
         */
        bool operator()(KDTreeEfficient *a, KDTreeEfficient *b) const {
            return sqDistanceFrom(a->area, queryPoint) > sqDistanceFrom(b->area, queryPoint);
        }
    };

    /**
     * Overloads output stream operator to print information of KD-Tree node
     * @param os
     * @param kdTreeEfficient
     * @return String representation of KD-Tree
     */
    friend std::ostream &operator<<(std::ostream &os, const KDTreeEfficient &kdTreeEfficient) {
        os << std::fixed << std::setprecision(1);

        return os << "A:" << kdTreeEfficient.area << "f:" << kdTreeEfficient.from << "t:" << kdTreeEfficient.to
                  << "xMed" << std::max(kdTreeEfficient.xMedian, 0.0) << "yMed"
                  << std::max(kdTreeEfficient.yMedian, 0.0) << "\n";
    }

private:
    Point *points;                  /**< The vector of points associated with the KD-Tree node. */
    Area area{};                    /**< The area covered by the SortKDTree node. */
    int from, to;                   /**< Lower bound of points, higher bound of points */
    KDTreeEfficient *leftChild{};   /**< Pointer to the left child of the SortKDTree node. */
    KDTreeEfficient *rightChild{};  /**< Pointer to the right child of the SortKDTree node. */
    double xMedian, yMedian;        /**< Median of x- / y-coordinate */

    /**
     * @brief private helper function to build the KD-Tree
     *
     * Uses level parameter to determine whether to split on X or Y-axis
     * @param lev
     */
    void buildTree(int level);

    /**
     * @Brief makes vertical split of the area und creates to children accordingly
     * @param lev level to decide split-coordinate
     */
    void setVerticalChildren(int level);

    /**
     * @Brief makes horizontal split of the area und creates to children accordingly
     * @param lev
     */
    void setHorizontalChildren(int level);

    /**
     * @brief Helper method for kNearestNeighbors(Point &queryPoint, int k). Adds k-nearest neighbor of KD-Tree node to result vector
     *
     * Recursively traverses KD-Tree nodes to find k nearest neighbors. Uses priority to keep track of proximity to query Point
     *
     * @param node The KD-Tree that is used for k-NNS
     * @param k The number of neighbors
     * @param queue Priority Queue that prioritizes nodes by proximity of its square to a queryPoint
     * @param result Vector that contains the k nearest neighbors
     */
    void kNearestNeighborsHelper(KDTreeEfficient *node, int k,
                                 priority_queue<KDTreeEfficient *, std::vector<KDTreeEfficient *>, CompareKDETree> &queue,
                                 std::vector<Point> &result);

public:

    /**
     * @Brief Constructs a KD-Tree with the specified area and points
     *
     * if the level of a node is even, the list is sorted using the X-Coordinate, otherwise Y-Coordinate is used
     *
     * @param points array of points
     * @param level current level
     * @param area containing all points
     * @param from lower bound of point array
     * @param to upper bound of point array
     */
    KDTreeEfficient(Point *points, int level, Area &area, int from, int to);

    /**
     * destroys the KD-Tree and deallocates memory
     */
    ~KDTreeEfficient();

    /**
     * Checks if given point is contained by the KD-Tree
     * @param point
     * @return True if KD-Tree contains point, false otherwise
     */
    bool contains(Point p);

    /**
     * @param queryRectangle Rectangle that contains points of interest
     * @return list<Point> of points contained by queryRectangle
     */
    list<Point> query(Area queryArea);

    /**
     * @brief private helper function to build the KD-Tree
     *
     * Uses level parameter to determine whether to split on X or Y-axis
     * @param lev
     */
    void buildTree();

    /**
     * @brief Calculates height of the Quadtree
     * @return The height of the Quadtree
     */
    int getHeight();

    /**
     * @brief Checks if this node is a leaf
     * @return True if node is a leaf, false otherwise
     */
    [[nodiscard]] bool isLeaf() const;

    /**
    * Get k nearest neighbors of a query point
    * @param queryPoint The point of which the k nearest neighbors are determined
    * @param k The number of neighbors
    * @return vector containing k nearest neighbors of queryPoint
    */
    vector<Point> kNearestNeighbors(Point &point, int k);
};

#endif //QUADKDBENCH_KDTREEEFFICIENT_H
