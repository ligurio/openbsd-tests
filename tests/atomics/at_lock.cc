#include "benchmark/benchmark.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#ifdef __OpenBSD__
#include <sys/futex.h>
#endif

// https://github.com/kmcallister/seqloq
// https://github.com/lcapaldo/futexexamples
// ftp://ftp.netbsd.org/pub/NetBSD/misc/gmcgarry/bench/lockprof.tar.gz
// https://git.kernel.org/pub/scm/linux/kernel/git/dvhart/futextest.git
// http://demin.ws/blog/english/2012/05/05/atomic-spinlock-mutex/
// https://github.com/linux-test-project/ltp/blob/master/testcases/open_posix_testsuite/conformance/interfaces/pthread_rwlock_init/1-1.c

void rwlock(int) {
}

void BM_rwlock(benchmark::State& state) {
	while (state.KeepRunning()) rwlock(state.range(0));
}

BENCHMARK_RANGE(BM_rwlock, 1, 10 * 10);

void futex(int) {
#ifdef __OpenBSD__
#endif
}

void BM_futex(benchmark::State& state) {
#ifdef __OpenBSD__
	while (state.KeepRunning()) futex(state.range(0));
#else
	        state.SkipWithError("Unsupported");
#endif
}

BENCHMARK_RANGE(BM_futex, 1, 10 * 10);

BENCHMARK_MAIN()
