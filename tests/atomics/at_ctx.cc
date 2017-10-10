#include "benchmark/benchmark.h"
#include <unistd.h>

/*
 * The system-call overhead benchmark measures the time for the operating
 * system to switch from unprivileged user mode to privileged kernel mode and
 * returning to unprivileged user mode. This metric is performed by measuring
 * the time to execute the getpid system call, which is the simplest system
 * call available in the operating system.
 *
 * The context-switch time benchmark measures the time for the operating system
 * to switch from the execution context of a process to another. A context
 * switch requires the kernel to save the address space and CPU registers of
 * the current process and load them for the next process. The kernel must also
 * manage coherency of the CPU caches. This metric is performed by creating two
 * processes connected with a bi-directional pipe. A one-byte token is passed
 * alternately between the two processes, causing a context switch to each
 * process to services access to the pipe. 
 *
 * https://github.com/tsuna/contextswitch
 * http://blog.tsunanet.net/2010/11/how-long-does-it-take-to-make-context.html
 *
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
