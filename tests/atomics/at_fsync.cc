#include "benchmark/benchmark.h"

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define B_SIZE    2048
#define B_NUM     100

/*
 * https://github.com/hostmaster/POC_fsyncperf/
 */

extern int errno ;

void fsync_perf() {

    int fdo, j;
    char buffer[B_SIZE];

    srand(time(NULL));

    for(j = 0; j < B_SIZE; j++) {
        buffer[j] = (char)(rand() & 0xFF);
    }

    char filename[] = "/tmp/at_fsync-XXXXXX";

    if ((fdo = mkstemp(filename)) == -1) {
        exit(errno);
    }

    for (j = 0; j < B_NUM; j++) {
        if (write(fdo, buffer, sizeof(buffer)) == -1) {
            exit(errno);
        }
    }

    if (fsync(fdo) == -1) {
        exit(errno);
    }

    if (close(fdo) == -1) {
        exit(errno);
    }

    unlink(filename);
}

void BM_fsync(benchmark::State& state) {
	  while (state.KeepRunning()) fsync_perf();
}

BENCHMARK(BM_fsync);

BENCHMARK_MAIN()
