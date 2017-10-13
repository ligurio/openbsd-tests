#include "benchmark/benchmark.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <err.h>

#define ITERATIONS	1000000
#define SIGNAL		SIGUSR1

volatile int iterations = ITERATIONS;

void handler(int v)
{
	iterations--;
}

static void sigbench()
{
	int cur, i;
	pid_t pid;

	pid = getpid();

	if (signal(SIGNAL, SIG_IGN) < 0)
		perror("cannot set signal handler");

	for (i=0; i<ITERATIONS; i++) {
		if (kill(pid, SIGNAL) < 0)
			perror("kill not possible");
	}

	if (signal(SIGNAL, handler) < 0)
		perror("cannot set signal handler");

	while ((cur = iterations) != 0) {
		if (kill(pid, SIGNAL) < 0)
			perror("kill not possible");
		while (cur == iterations)
			;
	}
}

void BM_sigbench(benchmark::State& state) {
	  while (state.KeepRunning()) sigbench();
}

BENCHMARK(BM_sigbench);

BENCHMARK_MAIN()
