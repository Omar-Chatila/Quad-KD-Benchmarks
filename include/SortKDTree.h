/**
 * @author Omar Chatila
 * @file SortKDTree.cpp
 * @brief Implementation of a KD-Tree
 *
 * This version of KD-Trees uses a sorting algorithm to determine the median
 * Building the tree in O(nlog(n)^2)
 */

#ifndef QUADKDBENCH_SORTKDTREE_H
#define QUADKDBENCH_SORTKDTREE_H

#include "Util.h"
#include <bits/stdc++.h>

using namespace std;

class SortKDTree {
    /**
     * @brief A comparison struct for prioritizing KD-Tree areas based on distance from a query point.
     */
    struct CompareMKDTree {
        const Point &queryPoint;

        /**
         * @brief Constructor for CompareMKDTree.
         * @param p The query point to calculate distances from.
         */
        explicit CompareMKDTree(const Point &p) : queryPoint(p) {}

        /**
         * @brief Overloaded operator to compare SortKDTree nodes based on squared distance from the query point.
         * @param a The first SortKDTree node.
         * @param b The second SortKDTree node.
         * @return True if the squared distance of point p to  node a is greater than to node b; false otherwise.
         */
        bool operator()(SortKDTree *a, SortKDTree *b) const {
            return sqDistanceFrom(a->area, queryPoint) > sqDistanceFrom(b->area, queryPoint);
        }
    };

private:
    Area area{};                 /**< The area covered by the SortKDTree node. */
    int level;                 /**< The level of the node node in the KD Tree. */
    vector<Point> points;      /**< The vector of points associated with the KD-Tree node. */
    SortKDTree *leftChild{};       /**< Pointer to the left child of the SortKDTree node. */
    SortKDTree *rightChild{};      /**< Pointer to the right child of the SortKDTree node. */

    /**
     * @Brief Constructs a KD-Tree with the specified area and points
     *
     * if the level of a node is even, the list is sorted using the X-Coordinate, otherwise Y-Coordinate is used
     *
     * @param points vector of points
     * @param area containing all points
     * @param level current level inside the tree
     */
    SortKDTree(vector<Point> &points, Area &area, int level);

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

    void appendPoint(Point &point, int level);

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
    void kNearestNeighborsHelper(SortKDTree *node, int k,
                                 priority_queue<SortKDTree *, std::vector<SortKDTree *>, CompareMKDTree> &queue,
                                 std::vector<Point> &result);

public:

    /**
     * @Brief Constructs a KD-Tree with the specified area and points
     *
     * if the level of a node is even, the list is sorted using the X-Coordinate, otherwise Y-Coordinate is used
     *
     * @param points vector of points
     * @param area containing all points
     * @param level current level inside the tree
     */
    SortKDTree(vector<Point> &points, Area &area);

    /**
     * destroys the KD-Tree and deallocates memory
     */
    ~SortKDTree();

    /**
     * @brief private helper function to build the KD-Tree
     *
     * Uses level parameter to determine whether to split on X or Y-axis
     * @param lev
     */
    void buildTree(int level);

    /**
     * @Brief Builds KD-Tree
     */
    void buildTree();

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
    list<Point> query(Area &queryArea);

    /**
     * @brief Calculates height of the Quadtree
     * @return The height of the Quadtree
     */
    int getHeight();

    /**
     * @brief Checks if this node is a leaf
     * @return True if node is a leaf, false otherwise
     */
    bool isLeaf();

    /**
     * @brief Adds a given Point to the KD-Tree
     * @param point Point to be added
     */
    void add(Point &point);

    /**
     * Get k nearest neighbors of a query point
     * @param queryPoint The point of which the k nearest neighbors are determined
     * @param k The number of neighbors
     * @return vector containing k nearest neighbors of queryPoint
     */
    vector<Point> kNearestNeighbors(Point &point, int k);
};

#endif //QUADKDBENCH_SORTKDTREE_H
