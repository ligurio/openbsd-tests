#include "benchmark/benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <err.h>
#include <unistd.h> 

/*
 *  The system-call overhead benchmark measures the time for the operating
 *  system to switch from unprivileged user mode to privileged kernel mode and
 *  returning to unprivileged user mode. This metric is performed by measuring
 *  the time to execute the getpid system call, which is the simplest system
 *  call available in the operating system.
 */

#define ITERATIONS	1000000

void BM_syscall_ohead(void) {

	struct timeval before, after;
	unsigned long overhead, elapsed;
	int i;
	pid_t pid;

	gettimeofday(&before, NULL);
	for (i=0; i<ITERATIONS; i++) {
	}
	gettimeofday(&after, NULL);
	overhead = 1000000 * (after.tv_sec - before.tv_sec);
	overhead += after.tv_usec - before.tv_usec;

	gettimeofday(&before, NULL);
	for (i=0; i<ITERATIONS; i++) {
		pid = getpid();
	}
	gettimeofday(&after, NULL);
	elapsed = 1000000 * (after.tv_sec - before.tv_sec);
	elapsed += after.tv_usec - before.tv_usec;

	printf("syscall time: %ld nanoseconds\n",
	    (1000*(elapsed-overhead))/ITERATIONS);
}

void BM_syscall_ohead(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(state.iterations());
	  }
}
BENCHMARK(BM_syscall_ohead);
BENCHMARK(BM_syscall_ohead)->ThreadPerCpu();

BENCHMARK_MAIN()
