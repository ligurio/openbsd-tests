#include "benchmark/benchmark.h"
#include <sys/socket.h>
#include <unistd.h>

void bind_perf() {

	/* FIXME */
}

void BM_bind(benchmark::State& state) {
	while (state.KeepRunning()) bind_perf();
}

BENCHMARK(BM_bind);

BENCHMARK_MAIN()
