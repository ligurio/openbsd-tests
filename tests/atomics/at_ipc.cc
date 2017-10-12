#include "benchmark/benchmark.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

// https://github.com/rigtorp/ipc-bench

void pipe_lat() {

	int ofds[2], ifds[2];
	int size;
	char *buf;
	int64_t i, count;

	size = 22222222; // message size: octets
	count = 333; // roundtrip count

	buf = (char*)malloc(size);
	if (buf == NULL) {
	  perror("malloc");
	}

	if (pipe(ofds) == -1) {
	  perror("pipe");
	}

	if (pipe(ifds) == -1) {
	  perror("pipe");
	}

	if (!fork()) { /* child */
	  for (i = 0; i < count; i++) {

	    if (read(ifds[0], buf, size) != size) {
	      perror("read");
	    }

	    if (write(ofds[1], buf, size) != size) {
	      perror("write");
	    }
	  }
	} else { /* parent */

		for (i = 0; i < count; i++) {

		  if (write(ifds[1], buf, size) != size) {
		    perror("write");
		  }

		  if (read(ofds[0], buf, size) != size) {
		    perror("read");
		  }
		}
	}
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
	while (state.KeepRunning()) pipe_lat();
}

BENCHMARK(BM_pipe_lat);

void BM_pipe_thr(benchmark::State& state) {
	while (state.KeepRunning()) pipe_thr();
}

BENCHMARK_RANGE(BM_pipe_thr, 1, 10 * 10);

void BM_unix_lat(benchmark::State& state) {
	while (state.KeepRunning()) unix_lat();
}

BENCHMARK_RANGE(BM_unix_lat, 1, 10 * 10);

void BM_unix_thr(benchmark::State& state) {
	while (state.KeepRunning()) unix_thr();
}

BENCHMARK_RANGE(BM_unix_thr, 1, 10 * 10);

void BM_tcp_lat(benchmark::State& state) {
	while (state.KeepRunning()) tcp_lat();
}

BENCHMARK_RANGE(BM_tcp_lat, 1, 10 * 10);

void BM_tcp_thr(benchmark::State& state) {
	while (state.KeepRunning()) tcp_thr();
}

BENCHMARK_RANGE(BM_tcp_thr, 1, 10 * 10);

BENCHMARK_MAIN()
