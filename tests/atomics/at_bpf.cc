#include "benchmark/benchmark.h"

/*
 * https://github.com/iovisor/bcc/tree/master/tests/cc
 * https://gist.github.com/msantos/939154
 *
 * BPF packet drop
 * BPF acquisition time (per packet)
 * BPF read overhead (2 cores)
 * BPF read overhead (1 core)
 * Packet drop ratio with read delay
 */
 
void bpf() {
}

void BM_bpf(benchmark::State& state) {
	while (state.KeepRunning()) bpf();
}

BENCHMARK(BM_bpf);

BENCHMARK_MAIN()
