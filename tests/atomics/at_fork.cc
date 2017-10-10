#include "benchmark/benchmark.h"
#include <unistd.h>
#include <sys/wait.h>

/* Benchmark considers the process creation and termination times as the
 * number of system processes increases. This benchmark measures the
 * architectural design of the scheduler.
 *
 * This benchmark creates a pipe and then opens many child processes, which
 * each write one byte into the pipe and then hang around until they are
 * killed. After creating each child, the benchmark waits until it can read the
 * one byte from the new child process from the pipe and takes the time from
 * the fork until having read the byte.
 */


void fork(int nforks) {
	int i, brksize;
	char *cp;
	int pid, child, status;
	struct timeval before, after;
	unsigned elapsed;

	brksize = 4;
	cp = (char *)sbrk(brksize);
	if (cp == (void *)-1) {
		//perror("sbrk");
		exit(4);
	}
	for (i = 0; i < brksize; i += 1024)
		cp[i] = i;

	for (i=0; i<nforks; i++) {
		child = fork();
		if (child == -1) {
			//perror("fork");
			exit(-1);
		}
		if (child == 0)
			_exit(-1);
		while ((pid = wait(&status)) != -1 && pid != child)
			;
	}
}

void BM_fork(benchmark::State& state) {
	while (state.KeepRunning()) fork(state.range(0));
}

BENCHMARK_RANGE(BM_fork, 1, 10 * 10);

BENCHMARK_MAIN()
