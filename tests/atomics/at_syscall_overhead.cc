#include "benchmark/benchmark.h"
#include <unistd.h> 

/*
 *  The system-call overhead benchmark measures the time for the operating
 *  system to switch from unprivileged user mode to privileged kernel mode and
 *  returning to unprivileged user mode. This metric is performed by measuring
 *  the time to execute the getpid system call, which is the simplest system
 *  call available in the operating system.
 */

#define ITERATIONS	1000000

void syscall_overhead(void) {

	int i;
	pid_t pid;

	for (i=0; i<ITERATIONS; i++) {
		pid = getpid();
	}
}

void BM_syscall_overhead(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(syscall_overhead);
	  }
}

BENCHMARK(BM_syscall_overhead);
BENCHMARK(BM_syscall_overhead)->Threads(8);
BENCHMARK(BM_syscall_overhead)->ThreadRange(1, 32);
BENCHMARK(BM_syscall_overhead)->ThreadPerCpu();

BENCHMARK_MAIN()
