#include "benchmark/benchmark.h"
#include <stdlib.h>

/*
 * https://people.freebsd.org/%7Ejkim/mmap_test.c
 * https://github.com/vext01/openbsd-libc-benchmarks
 */

void malloc_perf(unsigned long long bufsz) {

	char *buf;

	if ((buf = (char*)malloc(bufsz)) == 0)
		perror("malloc error");
	free(buf);
}

static void BM_malloc(benchmark::State& state) {
    while (state.KeepRunning()) malloc_perf(state.range(0));
}

BENCHMARK_RANGE(BM_malloc, 0, 10000 * 100000);

BENCHMARK_MAIN()
