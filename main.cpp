#include <iostream>
#include "QuadTree.h"
#include "MyKDTree.h"
#include "KDTreeEfficient.h"

#define FAST_IO() ios_base::sync_with_stdio(false); cin.tie(NULL)
#pragma GCC optimize("O3")
#pragma comment(linker, "/STACK:1000000000")

int main() {
    FAST_IO();
    std::cout << "Bench Build Time" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, 1000000);

    int pointNumber = 10000;
    Point pointArray[pointNumber];

    for (int i = 0; i < pointNumber; i++) {
        double x = dis(gen);
        double y = dis(gen);
        Point newPoint(x, y);
        pointArray[i] = newPoint;
    }


    Area area = Area(0, 1000000, 0, 1000000);
    cout << "Start" << "\n";

    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);

    KDTreeEfficient kdTreeEfficient(pointArray, 0, area, 0, pointNumber - 1);
    kdTreeEfficient.buildTree();

    clock_gettime(CLOCK_MONOTONIC, &now);
    printf("Elapsed: %lf miliseconds\n", (now.tv_sec - start.tv_sec) + 1E-6 * (now.tv_nsec - start.tv_nsec));

    cout << kdTreeEfficient.getHeight() << "\n";

    /*
        points.push_back(Point{2, 4});
        points.push_back(Point{5, 7});
        points.push_back(Point{1, 3});
        points.push_back(Point{8, 2});
        points.push_back(Point{6, 9});
        points.push_back(Point{3, 1});
        points.push_back(Point{7, 5});
        points.push_back(Point{4, 8});
        points.push_back(Point{9, 6});
        points.push_back(Point{0, 0});
     */


    /*vector<Point> queried = myKdTree.query(Area(1, 8, 3, 9));
    for (auto point: queried) {
        cout << point.x << " : " << point.y << "\n";
    }
     */
    return 0;
}
