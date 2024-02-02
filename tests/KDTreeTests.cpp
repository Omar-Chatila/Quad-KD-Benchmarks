//
// Created by omar on 02.02.24.
//

#include "../include/KDTreeEfficient.h"
#include "../include/SortKDTree.h"

namespace KDTreeTests {


    std::list<Point> naiveQuery(vector<Point> &points, Area &area) {
        std::list<Point> result;
        for (auto &p: points) {
            if (containsPoint(area, p)) {
                result.push_back(p);
            }
        }
        return result;
    }

    void testQuery() {
        Area area{0, 10000, 0, 10000};
        std::vector<Point> points1;
        points1.reserve(10000);
        auto *points = (Point *) (malloc(10000 * sizeof(Point)));
        for (int i = 0; i < 10000; ++i) {
            Point p{static_cast<double>(std::rand() % 10000), static_cast<double>(std::rand() % 10000)};
            if (find(points1.begin(), points1.end(), p) != points1.end()) {
                points1.push_back(p);
                points[i] = p;
            }
        }

        auto *pEfficient = new KDTreeEfficient(points, area, 10000);
        auto *sortKD = new SortKDTree(points1, area);
        pEfficient->buildTree();
        sortKD->buildTree();

        Area query3{500, 3939, 232, 23423};
        assert(pEfficient->query(query3) == naiveQuery(points1, query3));
        assert(sortKD->query(query3) == naiveQuery(points1, query3));


        std::vector<Area> areas(10000);
        for (auto &a: areas) {
            double fromX = std::rand() % 8000;
            double toX = fromX + std::rand() % 5000;
            double fromY = std::rand() % 8000;
            double toY = fromY + std::rand() % 5000;

            a = Area{fromX, toX, fromY, toY};
        }

        for (auto &a: areas) {
            std::list<Point> naive = naiveQuery(points1, a);
            assert(pEfficient->query(a) == naive);
            assert(sortKD->query(a) == naive);
        }
        delete pEfficient;
        delete sortKD;
    }
}

