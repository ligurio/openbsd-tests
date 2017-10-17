#include "benchmark/benchmark.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <err.h>

void sigusr1_sigaction(int v) {
	printf("");
}

static void sigbench() {
	pid_t pid;

	pid = getpid();
	if (signal(SIGUSR1, SIG_IGN) < 0)
		perror("cannot set signal handler");

	if (kill(pid, SIGUSR1) < 0)
		perror("kill not possible");

	if (signal(SIGUSR1, sigusr1_sigaction) < 0)
		perror("cannot set signal handler");

	if (kill(pid, SIGUSR1) < 0)
		perror("kill not possible");

	/*
	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigfpe_sigaction;
	sa.sa_flags = SA_NODEFER;
	sigaction(SIGFPE, &sa, NULL);
	*/
}

void BM_sigbench(benchmark::State& state) {
	  while (state.KeepRunning()) sigbench();
}

BENCHMARK(BM_sigbench);

BENCHMARK_MAIN()
