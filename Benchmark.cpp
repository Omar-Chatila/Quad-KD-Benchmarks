//
// Created by omarc on 13/01/2024.
//

#include "KDTreeEfficient.h"
#include "Util.h"

#include <benchmark/benchmark.h>

#define START 1000
#define END 100000

using namespace std;

static void buildTree(benchmark::State &state) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, 100000);

    int pointNumber = state.range(0);
    Point pointArray[pointNumber];

    for (int i = 0; i < pointNumber; i++) {
        double x = dis(gen);
        double y = dis(gen);
        Point newPoint(x, y);
        pointArray[i] = newPoint;
    }

    Area area{0, 100000, 0, 100000};

    for (auto _: state) {
        KDTreeEfficient kdTreeEfficient(pointArray, 0, area, 0, pointNumber - 1);
        benchmark::DoNotOptimize(kdTreeEfficient);
        kdTreeEfficient.buildTree();
    }

    state.SetComplexityN(state.range(0));
}

#define TEST(func) BENCHMARK(algo1)->Name("func")->DenseRange(START, END, STEPS);

BENCHMARK(buildTree)->Name("Build KD-Tree-Efficient")->RangeMultiplier(2)->Range(1000, END)->Complexity(
        benchmark::oN)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();