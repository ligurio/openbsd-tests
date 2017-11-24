#include "benchmark/benchmark.h"

void perf_2d() {

}

void BM_2d(benchmark::State& state) {
	while (state.KeepRunning()) perf_2d();
}

BENCHMARK(BM_2d);

BENCHMARK_MAIN()
