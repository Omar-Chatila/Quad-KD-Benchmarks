//
// Created by omarc on 13/01/2024.
//

#include "../include/KDTreeEfficient.h"
#include "../include/KDBTreeEfficient.h"
#include "../include/TreeHelper.h"
#include "../benchmark/include/benchmark/benchmark.h"
#include "cmath"

#define START 512
#define END 33'554'432
#define K_START 1
#define K_END 10'000
#define ITERATIONS 100

using namespace std;

static void buildKDETree(benchmark::State &state) {
    int pointNumber = state.range(0);
    auto *points = getRandomPointsArray(pointNumber);
    double bounds = pointNumber;
    Area area{0, bounds, 0, bounds};

    for ([[maybe_unused]] auto _: state) {
        auto *kdTreeEfficient = new KDTreeEfficient(points, area, pointNumber);
        benchmark::DoNotOptimize(kdTreeEfficient);
        kdTreeEfficient->buildTree();
        delete kdTreeEfficient;
    }
    state.SetComplexityN(state.range(0));
}

static void buildKDBTree(benchmark::State &state) {
    int pointNumber = state.range(0);
    auto *points = getRandomPointsArray(pointNumber);
    double bounds = pointNumber;
    Area area{0, bounds, 0, bounds};
    int capacity = (int) max(log10(pointNumber), 4.0);
    for ([[maybe_unused]] auto _: state) {
        auto *kdbTreeEfficient = new KDBTreeEfficient(points, 0, area, 0, pointNumber, capacity);
        benchmark::DoNotOptimize(kdbTreeEfficient);
        kdbTreeEfficient->buildTree();
        delete kdbTreeEfficient;
    }
    state.SetComplexityN(state.range(0));
}

static void buildQuadTree(benchmark::State &state) {
    int pointNumber = state.range(0);
    double bounds = pointNumber;
    vector<Point> points = getRandomPoints(pointNumber);

    Area area{0, bounds, 0, bounds};
    for ([[maybe_unused]] auto _: state) {
        auto *quadTree = new QuadTree(area, points);
        benchmark::DoNotOptimize(quadTree);
        quadTree->buildTree();
        delete quadTree;
    }
    state.SetComplexityN(state.range(0));
}

static void buildPRQuadTree(benchmark::State &state) {
    int pointNumber = state.range(0);
    int capacity = (int) max(log10(pointNumber), 4.0);
    double bounds = pointNumber;
    vector<Point> points = getRandomPoints(pointNumber);
    Area area{0, bounds, 0, bounds};
    for ([[maybe_unused]] auto _: state) {
        auto *prQuadTree = new PointRegionQuadTree(area, points, capacity);
        benchmark::DoNotOptimize(prQuadTree);
        prQuadTree->buildTree();
        delete prQuadTree;
    }
    state.SetComplexityN(state.range(0));
}

static void buildSKDTree(benchmark::State &state) {
    vector<Point> points = getRandomPoints(state.range(0));
    double bounds = state.range(0);
    Area area{0, bounds, 0, bounds};
    for ([[maybe_unused]] auto _: state) {
        auto *sortKDTree = new SortKDTree(points, area);
        benchmark::DoNotOptimize(sortKDTree);
        sortKDTree->buildTree();
        delete sortKDTree;
    }
    state.SetComplexityN(state.range(0));
}

static void buildDenseKDETree(benchmark::State &state) {
    int pointNumber = state.range(0);
    auto *points = getDensePointsArray(pointNumber);
    Area area{0, 1000, 0, 1000};

    for ([[maybe_unused]] auto _: state) {
        auto *kdTreeEfficient = new KDTreeEfficient(points, area, pointNumber);
        benchmark::DoNotOptimize(kdTreeEfficient);
        kdTreeEfficient->buildTree();
        delete kdTreeEfficient;
    }
    free(points);
    state.SetComplexityN(state.range(0));
}

static void buildDenseKDBTree(benchmark::State &state) {
    int pointNumber = state.range(0);
    auto *points = getDensePointsArray(pointNumber);
    Area area{0, 1000, 0, 1000};
    int capacity = (int) max(log10(pointNumber), 4.0);
    for ([[maybe_unused]] auto _: state) {
        auto *kdbTreeEfficient = new KDBTreeEfficient(points, 0, area, 0, pointNumber, capacity);
        benchmark::DoNotOptimize(kdbTreeEfficient);
        kdbTreeEfficient->buildTree();
        delete kdbTreeEfficient;
    }
    state.SetComplexityN(state.range(0));
}

static void buildDenseQuadTree(benchmark::State &state) {
    int pointNumber = state.range(0);
    vector<Point> points = getDensePoints(pointNumber);
    Area area{0, 1000, 0, 1000};
    for ([[maybe_unused]] auto _: state) {
        auto *quadTree = new QuadTree(area, points);
        benchmark::DoNotOptimize(quadTree);
        quadTree->buildTree();
        delete quadTree;
    }
    state.SetComplexityN(state.range(0));
}

static void buildDensePRQuadTree(benchmark::State &state) {
    int pointNumber = state.range(0);
    int capacity = (int) max(log10(pointNumber), 4.0);
    vector<Point> points = getDensePoints(pointNumber);
    Area area{0, 1000, 0, 1000};
    for ([[maybe_unused]] auto _: state) {
        auto *prQuadTree = new PointRegionQuadTree(area, points, capacity);
        benchmark::DoNotOptimize(prQuadTree);
        prQuadTree->buildTree();
        delete prQuadTree;
    }
    state.SetComplexityN(state.range(0));
}

static void buildDensesortKDTree(benchmark::State &state) {
    vector<Point> points = getDensePoints(state.range(0));
    Area area{0, 1000, 0, 1000};
    for ([[maybe_unused]] auto _: state) {
        auto *sortKDTree = new SortKDTree(points, area);
        benchmark::DoNotOptimize(sortKDTree);
        sortKDTree->buildTree();

        delete sortKDTree;
    }
    state.SetComplexityN(state.range(0));
}


// Build dynamically

static void buildKDTree_Dynamically(benchmark::State &state) {
    int size = state.range(0);
    vector<Point> points;
    double bounds = size;
    Area area{0, bounds, 0, bounds};
    auto *sortKDTree = new SortKDTree(points, area);
    vector<Point> randomPoints = getRandomPoints(size);
    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(sortKDTree);
        for (int i = 0; i < size; i++) {
            sortKDTree->add(randomPoints[i]);
        }
    }
    delete sortKDTree;
    state.SetComplexityN(state.range(0));
}

static void buildQuadTree_Dynamically(benchmark::State &state) {
    int size = state.range(0);
    vector<Point> points;
    points.reserve(size);
    double bounds = size;
    Area area{0, bounds, 0, bounds};
    auto *quadTree = new QuadTree(area, points);
    auto *randomPoints = (Point *) (malloc(size * sizeof(Point)));
    for (int i = 0; i < size; i++) {
        randomPoints[i] = getRandomPoint(size);
    }
    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(quadTree);
        for (int i = 0; i < size; i++) {
            quadTree->add(randomPoints[i]);
        }
    }
    delete quadTree;
    free(randomPoints);
    state.SetComplexityN(state.range(0));
}


static void queryQuadTree(benchmark::State &state) {
    int size = state.range(0);
    QuadTree *quadTree = buildQuadTreeRandom(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(quadTree);
        quadTree->query(bigArea);

    }
    delete quadTree;
    state.SetComplexityN(state.range(0));
}

static void queryPRQuadTree(benchmark::State &state) {
    int size = state.range(0);
    PointRegionQuadTree *quadTree = buildPRQuadTreeRandom(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(quadTree);
        quadTree->query(bigArea);
    }
    delete quadTree;
    state.SetComplexityN(state.range(0));
}

static void queryKDETree(benchmark::State &state) {
    int size = state.range(0);
    auto *points = getRandomPointsArray(size);
    double bounds = size;
    Area area{0, bounds, 0, bounds};
    auto *kdTreeEfficient = new KDTreeEfficient(points, area, size);
    kdTreeEfficient->buildTree();
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(kdTreeEfficient);
        kdTreeEfficient->query(bigArea);
    }
    delete kdTreeEfficient;
    state.SetComplexityN(state.range(0));
}

static void queryKDBTree(benchmark::State &state) {
    int size = state.range(0);
    auto *points = getRandomPointsArray(size);
    double bounds = size;
    int capacity = (int) max(log10(bounds), 4.0);
    Area area{0, bounds, 0, bounds};
    auto *kdbTreeEfficient = new KDBTreeEfficient(points, 0, area, 0, size, capacity);
    kdbTreeEfficient->buildTree();
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(kdbTreeEfficient);
        kdbTreeEfficient->query(bigArea);
    }
    delete kdbTreeEfficient;
    state.SetComplexityN(state.range(0));
}

static void querysortKDTree(benchmark::State &state) {
    int size = state.range(0);
    SortKDTree *sortKDTree = buildSortKDTreeRandom(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(sortKDTree);
        sortKDTree->query(bigArea);
    }
    delete sortKDTree;
    state.SetComplexityN(state.range(0));
}

static void queryNaive(benchmark::State &state) {
    int size = state.range(0);
    vector<Point> points = getRandomPoints(state.range(0));
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(getQueryNaive(points, bigArea));
    }
    points.clear();
    state.SetComplexityN(state.range(0));
}

static void quadTree_contains(benchmark::State &state) {
    int size = state.range(0);
    QuadTree *quadTree = buildQuadTreeRandom(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    searchPoints.reserve(size / 100);
    int step = size / 100;
    for (int i = 0; i < state.range(0); i += step) {
        searchPoints.push_back(points.at(i));
    }
    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(quadTree);
        qtContainsPoint(quadTree, searchPoints);
    }
    delete quadTree;
    state.SetComplexityN(state.range(0));
}

static void pr_quadTree_contains(benchmark::State &state) {
    int size = state.range(0);
    PointRegionQuadTree *quadTree = buildPRQuadTreeRandom(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    searchPoints.reserve(size / 100);
    int step = size / 100;
    for (int i = 0; i < state.range(0); i += step) {
        searchPoints.push_back(points.at(i));
    }

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(quadTree);
        pr_qtContainsPoint(quadTree, searchPoints);
    }
    delete quadTree;
    state.SetComplexityN(state.range(0));
}

static void kDTreeEfficient_Contains(benchmark::State &state) {
    int size = state.range(0);
    KDTreeEfficient *tree = buildEKD_Random(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < state.range(0); i += step) {
        searchPoints.push_back(points.at(i));
    }

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        kdEContainsPoint(tree, searchPoints);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void kDBTreeEfficient_Contains(benchmark::State &state) {
    int size = state.range(0);
    KDBTreeEfficient *tree = buildKDB_Random(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < state.range(0); i += step) {
        searchPoints.push_back(points.at(i));
    }

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        kdbContainsPoint(tree, searchPoints);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void sortKDTree_Contains(benchmark::State &state) {
    int size = state.range(0);
    SortKDTree *tree = buildSortKDTreeRandom(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < state.range(0); i += step) {
        searchPoints.push_back(points.at(i));
    }

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        sKDContainsPoint(tree, searchPoints);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void naive_Contains(benchmark::State &state) {
    int size = state.range(0);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < state.range(0); i += step) {
        searchPoints.push_back(points.at(i));
    }

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(searchPoints);
        benchmark::DoNotOptimize(points);
        benchmark::DoNotOptimize(containsNaive(searchPoints, points));
    }
    state.SetComplexityN(state.range(0));
}

static void sortKDTree_NNS(benchmark::State &state) {
    int size = state.range(0);
    SortKDTree *tree = buildSortKDTreeRandom(size);
    Point queryPoint{0.35 * size, 0.75 * size};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, 10);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void eKDTree_NNS(benchmark::State &state) {
    int size = state.range(0);
    KDTreeEfficient *tree = buildEKD_Random(size);
    Point queryPoint{0.35 * size, 0.75 * size};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, 10);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void KDBTree_NNS(benchmark::State &state) {
    int size = state.range(0);
    KDBTreeEfficient *tree = buildKDB_Random(size);
    Point queryPoint{0.35 * size, 0.75 * size};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, 10);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void quadTree_NNS(benchmark::State &state) {
    int size = state.range(0);
    QuadTree *tree = buildQuadTreeRandom(size);
    Point queryPoint{0.35 * size, 0.75 * size};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, 10);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void pr_quadTree_NNS(benchmark::State &state) {
    int size = state.range(0);
    PointRegionQuadTree *tree = buildPRQuadTreeRandom(size);
    Point queryPoint{0.35 * size, 0.75 * size};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, 10);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void naive_NNS(benchmark::State &state) {
    int size = state.range(0);
    std::vector<Point> points = getRandomPoints(size);
    Point queryPoint{0.35 * size, 0.75 * size};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(points);
        naive_kNNS(queryPoint, points, 10);
    }
    state.SetComplexityN(state.range(0));
}

static void sortKDTree_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 10'000'000;
    SortKDTree *tree = buildSortKDTreeRandom(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, k);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void eKDTree_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 10'000'000;
    KDTreeEfficient *tree = buildEKD_Random(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, k);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void KDBTree_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 10'000'000;
    KDBTreeEfficient *tree = buildKDB_Random(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, k);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void quadTree_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 10'000'000;
    QuadTree *tree = buildQuadTreeRandom(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, k);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void pr_quadTree_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 10'000'000;

    PointRegionQuadTree *tree = buildPRQuadTreeRandom(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, k);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void naive_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 10'000'000;
    std::vector<Point> points = getRandomPoints(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for ([[maybe_unused]] auto _: state) {
        benchmark::DoNotOptimize(points);
        benchmark::DoNotOptimize(naive_kNNS(queryPoint, points, k));
    }
    state.SetComplexityN(state.range(0));
}

// BUILD
BENCHMARK(buildQuadTree)
        ->Name("Build Quadtree")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildPRQuadTree)
        ->Name("Build PR-Quadtree")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildSKDTree)
        ->Name("Build SortKDTree")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildKDETree)
        ->Name("Build KD-Tree-Efficient")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildKDBTree)
        ->Name("Build KDB-Tree-Efficient")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildDenseQuadTree)
        ->Name("Build Dense Quadtree")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildDensePRQuadTree)
        ->Name("Build Dense PR-Quadtree")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildDensesortKDTree)
        ->Name("Build Dense SortKDTree")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildDenseKDETree)
        ->Name("Build Dense KD-Tree-Efficient")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildDenseKDBTree)
        ->Name("Build Dense KDB-Tree-Efficient")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

// Build dynamically
/*BENCHMARK(buildKDTree_Dynamically)
        ->Name("Build SortKDTree-dynamically")
        ->RangeMultiplier(2)->Range(START, END)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);

BENCHMARK(buildQuadTree_Dynamically)
        ->Name("Build Quadtree-dynamically")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(ITERATIONS);
        */

// Query
BENCHMARK(queryQuadTree)
        ->Name("Query Quadtree - Variable PointCount")
        ->RangeMultiplier(2)->Range(START, END)
        ->Unit(benchmark::kMicrosecond)
        ->Complexity(benchmark::oAuto)
        ->Iterations(ITERATIONS);

BENCHMARK(queryPRQuadTree)
        ->Name("Query PR-Quadtree - Variable PointCount")
        ->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(querysortKDTree)
        ->Name("Query SortKDTree - Variable PointCount")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(queryKDETree)
        ->Name("Query KD-E - Variable PointCount")
        ->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(queryKDBTree)
        ->Name("Query KDB-E - Variable PointCount")
        ->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(queryNaive)->Name("Query Naive - Variable PointCount")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

// CONTAINS
BENCHMARK(pr_quadTree_contains)
        ->Name("PR-Quadtree - Contains")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(sortKDTree_Contains)
        ->Name("SortKDTree - Contains")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(kDBTreeEfficient_Contains)
        ->Name("KDB-tree - Contains")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(quadTree_contains)
        ->Name("Quadtree - Contains")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(kDTreeEfficient_Contains)
        ->Name("KD_Tree_Efficient - Contains")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(naive_Contains)->Name("Naive - Contains")
        ->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);


// NNS - variable point count k = 10
BENCHMARK(quadTree_NNS)
        ->Name("Quadtree - NNS - var n")
        ->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(pr_quadTree_NNS)
        ->Name("PR-Quadtree - NNS - var n")
        ->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(sortKDTree_NNS)
        ->Name("SortKDTree - NNS - var n")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(eKDTree_NNS)
        ->Name("KD_Tree_Efficient -- NNS - var n")
        ->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(KDBTree_NNS)
        ->Name("KDB_Tree -- NNS - var n")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(naive_NNS)
        ->Name("Naive - NNS - var n")
        ->RangeMultiplier(2)
        ->Range(START, END)
        ->Complexity(benchmark::oN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

// k-NNS - variable k
BENCHMARK(quadTree_kNNS)
        ->Name("Quadtree - NNS - var k")
        ->RangeMultiplier(10)
        ->Range(K_START, K_END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(pr_quadTree_kNNS)
        ->Name("PR-Quadtree - NNS - var k")
        ->RangeMultiplier(10)
        ->Range(K_START, K_END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(eKDTree_kNNS)
        ->Name("KD_Tree_Efficient -- NNS - var k")
        ->RangeMultiplier(10)
        ->Range(K_START, K_END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(KDBTree_kNNS)
        ->Name("KDB_Tree -- NNS - var k")
        ->RangeMultiplier(10)
        ->Range(K_START, K_END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(sortKDTree_kNNS)
        ->Name("SortKDTree - NNS - var k")
        ->RangeMultiplier(10)
        ->Range(K_START, K_END)
        ->Complexity(benchmark::oAuto)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK(naive_kNNS)
        ->Name("Naive - NNS - var k")
        ->RangeMultiplier(10)
        ->Range(K_START, K_END)
        ->Complexity(benchmark::oNLogN)
        ->Unit(benchmark::kMicrosecond)
        ->Iterations(ITERATIONS);

BENCHMARK_MAIN();
