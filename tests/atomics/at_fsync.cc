#include "benchmark/benchmark.h"

// https://github.com/hostmaster/POC_fsyncperf/blob/master/fsync_test.c

void BM_hello(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(state.iterations());
	  }
}
BENCHMARK(BM_hello);
BENCHMARK(BM_hello)->ThreadPerCpu();

BENCHMARK_MAIN()
