#include "benchmark/benchmark.h"

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define B_SIZE    2048
#define B_NUM     100
#define MAX_FILES 10000

extern int errno ;

void fsync_perf() {
    int fdo, i, j;
    ssize_t len;
    unsigned long long total = 0;

    char buffer[B_SIZE];
    unsigned long long times[MAX_FILES];

    srand(time(NULL));

    for(i = 0; i < MAX_FILES; i++) {

        for(j = 0; j < B_SIZE; j++) {
            buffer[j] = (char)(rand() & 0xFF);
        }

        char filename[] = "fsync-bench-XXXXXX";

        if ((fdo = mkstemp(filename)) == -1) {
            perror("mkstemp");
            // exit(errno);
        }

        for (j = 0; j < B_NUM; j++) {
            if (write(fdo, buffer, sizeof(buffer)) == -1) {
                perror("write");
                // exit(errno);
            }
        }

        if (fsync(fdo) == -1) {
            perror("fsync");
            // exit(errno);
        }

        if (close(fdo) == -1) {
            perror("close");
            // exit(errno);
        }
        unlink(filename);
    }
}

void BM_fsync(benchmark::State& state) {
	  while (state.KeepRunning()) fsync_perf;
}

BENCHMARK(BM_fsync);

BENCHMARK_MAIN()
