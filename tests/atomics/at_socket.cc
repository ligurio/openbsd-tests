#include "benchmark/benchmark.h"
#include <unistd.h>
#include <sys/wait.h>

void socket_perf() {

}

void BM_socket(benchmark::State& state) {
	while (state.KeepRunning()) socket_perf();
}

BENCHMARK(BM_socket);

BENCHMARK_MAIN()
