#include "benchmark/benchmark.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <err.h>

#define ITERATIONS	1000000
#define MYSIGNAL	SIGUSR1

volatile int iterations = ITERATIONS;


void
handler(int v)
{
	iterations--;
}

int
main(int argc, char *argv[])
{
	int cur, i;
	struct timeval before, after;
	unsigned long elapsed, overhead;
	pid_t pid;

	pid = getpid();

	if (signal(MYSIGNAL, SIG_IGN) < 0)
		errx(1, "cannot set signal handler");

	gettimeofday(&before, NULL);
	for (i=0; i<ITERATIONS; i++) {
		if (kill(pid, MYSIGNAL) < 0)
			errx(1, "kill not possible");
	}
	gettimeofday(&after, NULL);
	overhead = 1000000 * (after.tv_sec - before.tv_sec);
	overhead += (after.tv_usec - before.tv_usec);

	if (signal(MYSIGNAL, handler) < 0)
		errx(1, "cannot set signal handler");

	gettimeofday(&before, NULL);
	while ((cur = iterations) != 0) {
		if (kill(pid, MYSIGNAL) < 0)
			errx(1, "kill not possible");
		while (cur == iterations)
			;
	}
	gettimeofday(&after, NULL);

	elapsed = 1000000 * (after.tv_sec - before.tv_sec);
	elapsed += (after.tv_usec - before.tv_usec);
	printf ("Time: %ld nanoseconds.\n",
	    (1000*(elapsed-overhead))/ITERATIONS);

	return (0);
}

#define BASIC_BENCHMARK_TEST(x) BENCHMARK(x)->Arg(8)->Arg(512)->Arg(8192)

void BM_hello(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(state.iterations());
	  }
}
BENCHMARK(BM_hello);
BENCHMARK(BM_hello)->ThreadPerCpu();

BENCHMARK_MAIN()
