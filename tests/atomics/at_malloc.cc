#include "benchmark/benchmark.h"
#include <stdlib.h>

/*
 * https://people.freebsd.org/%7Ejkim/mmap_test.c
 * https://github.com/vext01/openbsd-libc-benchmarks
 */

void mmalloc() {

	char	*buf;
	unsigned long long	bufsz;

	bufsz = 32;
	if (!malloc(bufsz))
		exit(4);
}

static void BM_malloc(benchmark::State& state) {
    while (state.KeepRunning()) {
		benchmark::DoNotOptimize(mmalloc);
    }
}

BENCHMARK(BM_malloc);

BENCHMARK_MAIN()
