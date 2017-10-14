#include "benchmark/benchmark.h"
#include <unistd.h>
#include <sys/wait.h>

/*
 *  We are ultimately interested in the performance of HTTP requests. There are
 *  two parts to that: the connect latency and the latency for answering the
 *  actual HTTP request. The connect latency is the time it takes for the
 *  server to notice the connection attempt and call accept.
 *
 *  This time is largely dominated by the event notification. Accepting a
 *  connection does not actually do anything besides sending a TCP packet and
 *  allocating a file descriptor. The socket benchmark already showed that
 *  allocating a file descriptor is O(1) for each OS in the test. So it is
 *  reasonable to expect this benchmark to show that the operating systems with
 *  special event notification APIs scale O(1) (Linux 2.4: SIGIO, Linux 2.6:
 *  epoll, FreeBSD+OpenBSD: kqueue) and the rest to scale O(n) (NetBSD). My
 *  benchmark http server is called gatling and it makes use of SIGIO, epoll
 *  and kqueue if available, but falls back to poll if not.
 *
 */

void connect_perf() {

	int i, nforks;
	int pid, child, status;

	nforks = 2;
	for (i=0; i<nforks; i++) {
		child = fork();
		if (child == -1) {
			perror("fork");
			exit(-1);
		}
		if (child == 0)
			exit(-1);
		while ((pid = wait(&status)) != -1 && pid != child)
			;
	}
}

void BM_connect(benchmark::State& state) {
	while (state.KeepRunning()) connect_perf();
}

BENCHMARK(BM_connect);

BENCHMARK_MAIN()
