#include "benchmark/benchmark.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>

/*  Benchmark considers the process creation and termination times as the
 *  number of system processes increases. This benchmark measures the
 *  architectural design of the scheduler.
 */

/*
int nforks, i;
char *cp;
int pid, child, status, brksize;
struct timeval before, after;
unsigned elapsed;

nforks = atoi(argv[1]);
brksize = atoi(argv[2]);

gettimeofday(&before, NULL);
cp = (char *)sbrk(brksize);
if (cp == (void *)-1) {
        perror("sbrk");
        exit(4);
}
for (i = 0; i < brksize; i += 1024)
        cp[i] = i;
for (i=0; i<nforks; i++) {
        child = fork();
        if (child == -1) {
                perror("fork");
                exit(-1);
        }
        if (child == 0)
                _exit(-1);
        while ((pid = wait(&status)) != -1 && pid != child)
                ;
}
gettimeofday(&after, NULL);
elapsed = 1000000 * (after.tv_sec - before.tv_sec);
elapsed += (after.tv_usec - before.tv_usec);
printf ("Time: %ud microseconds.\n", elapsed/nforks);
*/

void BM_fork(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(state.iterations());
	  }
}
BENCHMARK(BM_fork);
BENCHMARK(BM_fork)->ThreadPerCpu();

BENCHMARK_MAIN()
