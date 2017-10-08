#include "benchmark/benchmark.h"

#define BASIC_BENCHMARK_TEST(x) BENCHMARK(x)->Arg(8)->Arg(512)->Arg(8192)

void BM_hello(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(state.iterations());
	  }
}
BENCHMARK(BM_hello);
BENCHMARK(BM_hello)->ThreadPerCpu();

BENCHMARK_MAIN()
