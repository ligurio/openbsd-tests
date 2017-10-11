#include "benchmark/benchmark.h"

// https://github.com/rigtorp/ipc-bench

void pipe_lat() {
}

void pipe_thr() {
}

void unix_lat() {
}

void unix_thr() {
}

void tcp_lat() {
}

void tcp_thr() {
}

void BM_pipe_lat(benchmark::State& state) {
	while (state.KeepRunning()) pipe_lat;
}

BENCHMARK_RANGE(BM_pipe_lat, 1, 10 * 10);

void BM_pipe_thr(benchmark::State& state) {
	while (state.KeepRunning()) pipe_thr;
}

BENCHMARK_RANGE(BM_pipe_thr, 1, 10 * 10);

void BM_unix_lat(benchmark::State& state) {
	while (state.KeepRunning()) unix_lat;
}

BENCHMARK_RANGE(BM_unix_lat, 1, 10 * 10);

void BM_unix_thr(benchmark::State& state) {
	while (state.KeepRunning()) unix_thr;
}

BENCHMARK_RANGE(BM_unix_thr, 1, 10 * 10);

void BM_tcp_lat(benchmark::State& state) {
	while (state.KeepRunning()) tcp_lat;
}

BENCHMARK_RANGE(BM_tcp_lat, 1, 10 * 10);

void BM_tcp_thr(benchmark::State& state) {
	while (state.KeepRunning()) tcp_thr;
}

BENCHMARK_RANGE(BM_tcp_thr, 1, 10 * 10);

BENCHMARK_MAIN()
