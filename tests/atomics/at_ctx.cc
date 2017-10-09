#include "benchmark/benchmark.h"

/*
 * The context-switch time benchmark measures the time for the operating system
 * to switch from the execution context of a process to another. A context
 * switch requires the kernel to save the address space and CPU registers of
 * the current process and load them for the next process. The kernel must also
 * manage coherency of the CPU caches. This metric is performed by creating two
 * processes connected with a bi-directional pipe. A one-byte token is passed
 * alternately between the two processes, causing a context switch to each
 * process to services access to the pipe. 
 */

// https://github.com/tsuna/contextswitch
// http://blog.tsunanet.net/2010/11/how-long-does-it-take-to-make-context.html

void BM_hello(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(state.iterations());
	  }
}
BENCHMARK(BM_hello);
BENCHMARK(BM_hello)->ThreadPerCpu();

BENCHMARK_MAIN()
