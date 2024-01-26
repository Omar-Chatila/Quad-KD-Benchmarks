//
// Created by omarc on 13/01/2024.
//

#include "../include/KDTreeEfficient.h"
#include "../include/TreeHelper.h"
#include "../include/KDBTreeEfficient.h"
#include "../include/QuadTree.h"
#include "../include/Util.h"

#include "../benchmark/include/benchmark/benchmark.h"
#include "../include/PointRegionQuadTree.h"
#include "cmath"

#define START 512
#define END 2'097'152
#define K_START 1
#define K_END 10'000
#define ITERATIONS 1

using namespace std;

static void buildKDETree(benchmark::State &state) {
    int pointNumber = state.range(0);
    auto *points = getRandomPointsArray(pointNumber);
    double bounds = pointNumber;
    Area area{0, bounds, 0, bounds};

    for (auto _: state) {
        auto *kdTreeEfficient = new KDTreeEfficient(points, 0, area, 0, pointNumber);
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
    for (auto _: state) {
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
    for (auto _: state) {
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
    for (auto _: state) {
        auto *prQuadTree = new PointRegionQuadTree(area, points, capacity);
        benchmark::DoNotOptimize(prQuadTree);
        prQuadTree->buildTree();
        delete prQuadTree;
    }
    state.SetComplexityN(state.range(0));
}

static void buildMyKDTree(benchmark::State &state) {
    vector<Point> points = getRandomPoints(state.range(0));
    Area area{0, 100000, 0, 100000};
    for (auto _: state) {
        auto *myKdTree = new MyKDTree(points, area, 0);
        benchmark::DoNotOptimize(myKdTree);
        myKdTree->buildTree();

        delete myKdTree;
    }
    state.SetComplexityN(state.range(0));
}

// Build dynamically

static void buildKDTree_Dynamically(benchmark::State &state) {
    int size = state.range(0);
    vector<Point> points;
    double bounds = size;
    Area area{0, bounds, 0, bounds};
    MyKDTree myKdTree(points, area, 0);
    auto *randomPoints = (Point *) (malloc(size * sizeof(Point)));
    for (int i = 0; i < size; i++) {
        randomPoints[i] = getRandomPoint(size);
    }
    for (auto _: state) {
        benchmark::DoNotOptimize(myKdTree);
        for (int i = 0; i < size; i++) {
            myKdTree.add(randomPoints[i]);
        }
    }
    free(randomPoints);
    state.SetComplexityN(state.range(0));
}

static void buildQuadTree_Dynamically(benchmark::State &state) {
    int size = state.range(0);
    vector<Point> points;
    points.reserve(size);
    double bounds = size;
    Area area{0, bounds, 0, bounds};
    QuadTree quadTree(area, points);
    auto *randomPoints = (Point *) (malloc(size * sizeof(Point)));
    for (int i = 0; i < size; i++) {
        randomPoints[i] = getRandomPoint(size);
    }
    for (auto _: state) {
        benchmark::DoNotOptimize(quadTree);
        for (int i = 0; i < size; i++) {
            quadTree.add(randomPoints[i]);
        }
    }
    free(randomPoints);
    state.SetComplexityN(state.range(0));
}


static void queryQuadTree(benchmark::State &state) {
    int size = state.range(0);
    QuadTree *quadTree = buildQuadTreeRandom(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for (auto _: state) {
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
    for (auto _: state) {
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
    auto *kdTreeEfficient = new KDTreeEfficient(points, 0, area, 0, size);
    kdTreeEfficient->buildTree();
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for (auto _: state) {
        benchmark::DoNotOptimize(kdTreeEfficient);
        kdTreeEfficient->query(bigArea);
    }
    delete kdTreeEfficient;
    state.SetComplexityN(state.range(0));
}

static void queryMYKDTree(benchmark::State &state) {
    int size = state.range(0);
    MyKDTree *myKdTree = buildMyKD_Random(size);
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for (auto _: state) {
        benchmark::DoNotOptimize(myKdTree);
        myKdTree->query(bigArea);
    }
    delete myKdTree;
    state.SetComplexityN(state.range(0));
}

static void queryNaive(benchmark::State &state) {
    int size = state.range(0);
    vector<Point> points = getRandomPoints(state.range(0));
    Area bigArea{0.3 * size, 0.5 * size, 0.54 * size, 0.64 * size};
    for (auto _: state) {
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
    for (auto _: state) {
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

    for (auto _: state) {
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

    for (auto _: state) {
        benchmark::DoNotOptimize(tree);
        kdEContainsPoint(tree, searchPoints);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void myKDTree_Contains(benchmark::State &state) {
    int size = state.range(0);
    MyKDTree *tree = buildMyKD_Random(size);
    std::vector<Point> points = getRandomPoints(size);
    std::vector<Point> searchPoints;
    int step = size / 100;
    for (int i = 0; i < state.range(0); i += step) {
        searchPoints.push_back(points.at(i));
    }

    for (auto _: state) {
        benchmark::DoNotOptimize(tree);
        myKdContainsPoint(tree, searchPoints);
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

    for (auto _: state) {
        benchmark::DoNotOptimize(searchPoints);
        benchmark::DoNotOptimize(points);
        benchmark::DoNotOptimize(containsNaive(searchPoints, points));
    }
    state.SetComplexityN(state.range(0));
}

static void myKDTree_NNS(benchmark::State &state) {
    int size = state.range(0);
    MyKDTree *tree = buildMyKD_Random(size);
    Point queryPoint{0.35 * size, 0.75 * size};

    for (auto _: state) {
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

    for (auto _: state) {
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

    for (auto _: state) {
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

    for (auto _: state) {
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

    for (auto _: state) {
        benchmark::DoNotOptimize(points);
        naive_kNNS(queryPoint, points, 10);
    }
    state.SetComplexityN(state.range(0));
}

static void myKDTree_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 1'000'000;
    MyKDTree *tree = buildMyKD_Random(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for (auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, k);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void eKDTree_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 1'000'000;
    KDTreeEfficient *tree = buildEKD_Random(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for (auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, k);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void quadTree_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 1'000'000;
    QuadTree *tree = buildQuadTreeRandom(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for (auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, k);
    }
    state.SetComplexityN(state.range(0));
}

static void pr_quadTree_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 1'000'000;

    PointRegionQuadTree *tree = buildPRQuadTreeRandom(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for (auto _: state) {
        benchmark::DoNotOptimize(tree);
        tree->kNearestNeighbors(queryPoint, k);
    }
    delete tree;
    state.SetComplexityN(state.range(0));
}

static void naive_kNNS(benchmark::State &state) {
    int k = state.range(0);
    int n = 1'000'000;
    std::vector<Point> points = getRandomPoints(n);
    Point queryPoint{0.35 * n, 0.75 * n};

    for (auto _: state) {
        benchmark::DoNotOptimize(points);
        naive_kNNS(queryPoint, points, k);
    }
    state.SetComplexityN(state.range(0));
}


#define TEST(func) BENCHMARK(algo1)->Name("func")->DenseRange(START, END, STEPS);

// BUILD
BENCHMARK(buildQuadTree)->Name("Build Quadtree")->RangeMultiplier(2)->Range(START, END)->Complexity(
        benchmark::oNLogN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(buildPRQuadTree)->Name("Build PR-Quadtree")->RangeMultiplier(2)->Range(START, END)->Complexity(
        benchmark::oNLogN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(buildMyKDTree)->Name("Build MyKDTree")->RangeMultiplier(2)->Range(START, END)->Complexity(
        benchmark::oNLogN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(buildKDETree)->Name("Build KD-Tree-Efficient")->RangeMultiplier(2)->Range(START, END)->Complexity(
        benchmark::oNLogN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(buildKDBTree)->Name("Build KDB-Tree-Efficient")->RangeMultiplier(2)->Range(START, END)->Complexity(
        benchmark::oNLogN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);

// Build dynamically

//BENCHMARK(buildQuadTree_Dynamically)->Name("Build Quadtree-dynamically")->RangeMultiplier(2)->Range(START, END)
//    ->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);

//BENCHMARK(buildKDTree_Dynamically)->Name("Build MyKDTree-dynamically")->RangeMultiplier(2)->Range(START, END)
//      ->Unit(benchmark::kMillisecond);

// Query
BENCHMARK(queryQuadTree)->Name("Query Quadtree - Variable PointCount")->RangeMultiplier(2)->Range(START, END)
        ->Unit(benchmark::kMillisecond)->Complexity(benchmark::oAuto)->Iterations(ITERATIONS);
BENCHMARK(queryPRQuadTree)->Name("Query PR-Quadtree - Variable PointCount")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oAuto)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(queryMYKDTree)->Name("Query MyKDTree - Variable PointCount")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oAuto)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(queryKDETree)->Name("Query KD-E - Variable PointCount")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oAuto)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(queryNaive)->Name("Query Naive - Variable PointCount")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oAuto)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);

// CONTAINS
BENCHMARK(quadTree_contains)->Name("Quadtree - Contains")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(pr_quadTree_contains)->Name("PR-Quadtree - Contains")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(kDTreeEfficient_Contains)->Name("KD_Tree_Efficient - Contains")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(myKDTree_Contains)->Name("MyKDTree - Contains")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);
BENCHMARK(naive_Contains)->Name("Naive - Contains")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);

// NNS - variable point count k = 10
BENCHMARK(quadTree_NNS)->Name("Quadtree - NNS - var n")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)->Unit(benchmark::kMicrosecond)->Iterations(ITERATIONS);
BENCHMARK(pr_quadTree_NNS)->Name("PR-Quadtree - NNS - var n")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)->Unit(benchmark::kMicrosecond)->Iterations(ITERATIONS);
BENCHMARK(eKDTree_NNS)->Name("KD_Tree_Efficient -- NNS - var n")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)->Unit(benchmark::kMicrosecond)->Iterations(ITERATIONS);
BENCHMARK(myKDTree_NNS)->Name("MyKDTree - NNS - var n")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oLogN)->Unit(benchmark::kMicrosecond)->Iterations(ITERATIONS);
BENCHMARK(naive_NNS)->Name("Naive - NNS - var n")->RangeMultiplier(2)->Range(START, END)
        ->Complexity(benchmark::oN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);

// k-NNS - variable k
BENCHMARK(quadTree_kNNS)->Name("Quadtree - NNS - var k")->RangeMultiplier(10)->Range(K_START, K_END)
        ->Complexity(benchmark::oAuto)->Unit(benchmark::kMicrosecond)->Iterations(ITERATIONS);
BENCHMARK(pr_quadTree_kNNS)->Name("PR-Quadtree - NNS - var k")->RangeMultiplier(10)->Range(K_START, K_END)
        ->Complexity(benchmark::oAuto)->Unit(benchmark::kMicrosecond)->Iterations(ITERATIONS);
BENCHMARK(eKDTree_kNNS)->Name("KD_Tree_Efficient -- NNS - var k")->RangeMultiplier(10)->Range(K_START, K_END)
        ->Complexity(benchmark::oAuto)->Unit(benchmark::kMicrosecond)->Iterations(ITERATIONS);
BENCHMARK(myKDTree_kNNS)->Name("MyKDTree - NNS - var k")->RangeMultiplier(10)->Range(K_START, K_END)
        ->Complexity(benchmark::oAuto)->Unit(benchmark::kMicrosecond)->Iterations(ITERATIONS);
BENCHMARK(naive_kNNS)->Name("Naive - NNS - var k")->RangeMultiplier(10)->Range(K_START, K_END)
        ->Complexity(benchmark::oN)->Unit(benchmark::kMillisecond)->Iterations(ITERATIONS);

BENCHMARK_MAIN();
