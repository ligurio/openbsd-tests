#include "benchmark/benchmark.h"
#include <stdlib.h>

void malloc_perf() {

	void *p;
	int size = 1 * 1024 * 1024;

    p = malloc(size);
	if (!p)
		perror("malloc error");
	free(p);
}

static void BM_malloc(benchmark::State& state) {
    while (state.KeepRunning()) malloc_perf();
}

BENCHMARK(BM_malloc);

BENCHMARK_MAIN()
