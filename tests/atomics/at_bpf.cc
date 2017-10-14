#include "benchmark/benchmark.h"

/*
 * https://github.com/iovisor/bcc/tree/master/tests/cc
 * https://gist.github.com/msantos/939154
 */
 
void bpf() {
}

void BM_bpf(benchmark::State& state) {
	while (state.KeepRunning()) bpf();
}

BENCHMARK(BM_bpf);

BENCHMARK_MAIN()
