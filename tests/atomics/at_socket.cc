#include "benchmark/benchmark.h"
#include <sys/socket.h>
#include <stdio.h>

void socket_perf() {

    socket(AF_INET , SOCK_STREAM , 0);
}

void BM_socket(benchmark::State& state) {
	while (state.KeepRunning()) socket_perf();
}

BENCHMARK(BM_socket);

BENCHMARK_MAIN()
