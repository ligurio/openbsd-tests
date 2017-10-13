#include "benchmark/benchmark.h"
#include <unistd.h>
#include <sys/wait.h>

void bind_perf() {

}

void BM_bind(benchmark::State& state) {
	while (state.KeepRunning()) bind_perf();
}

BENCHMARK(BM_bind);

BENCHMARK_MAIN()
