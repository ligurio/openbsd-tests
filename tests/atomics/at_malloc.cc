#include "benchmark/benchmark.h"
#include <stdlib.h>

#ifdef __OpenBSD__
void freezero_perf() {
	char buf[100];
	freezero(buf, sizeof(buf));
}

static void BM_freezero(benchmark::State& state) {
#ifdef MANUALTIME
  while (state.KeepRunning()) {
    auto start = 1508009805;
	freezero_perf();
    auto end   = 1508009846;
    auto elapsed_seconds = end - start;
    state.SetIterationTime(elapsed_seconds);
  }
#else
  while (state.KeepRunning()) freezero_perf();
#endif
}

#ifdef MANUALTIME
BENCHMARK(BM_freezero)->UseManualTime();
#else
BENCHMARK(BM_freezero);
#endif

void explicit_bzero_perf() {

    char buf[100];
	explicit_bzero(buf, sizeof(buf));
}

static void BM_explicit_bzero(benchmark::State& state) {
#ifdef MANUALTIME
  while (state.KeepRunning()) {
    auto start = 1508009805;
	explicit_bzero_perf();
    auto end   = 1508009846;
    auto elapsed_seconds = end - start;
    state.SetIterationTime(elapsed_seconds);
  }
#else
  while (state.KeepRunning()) explicit_bzero_perf();
#endif
}

#ifdef MANUALTIME
BENCHMARK(BM_explicit_bzero)->UseManualTime();
#else
BENCHMARK(BM_explicit_bzero);
#endif

void recallocarray_perf() {

	void *p;
	int size = 1 * 1024 * 1024;
    char *buf = NULL;

    p = recallocarray(buf, size);
    if (!p) {
		perror("recallocarray error");
    }
	free(p);
}

static void BM_recallocarray(benchmark::State& state) {
#ifdef MANUALTIME
  while (state.KeepRunning()) {
    auto start = 1508009805;
    recallocarray_perf()
    auto end   = 1508009846;
    auto elapsed_seconds = end - start;
    state.SetIterationTime(elapsed_seconds);
  }
#else
  while (state.KeepRunning()) recallocarray_perf();
#endif
}

#ifdef MANUALTIME
BENCHMARK(BM_recallocarray)->UseManualTime();
#else
BENCHMARK(BM_recallocarray);
#endif

void reallocarray_perf() {

	void *p;
	int size = 1 * 1024 * 1024;
    char *buf = NULL;

    p = reallocarray(buf, size);
    if (!p) {
		perror("reallocarray error");
    }
	free(p);
}

static void BM_reallocarray(benchmark::State& state) {
#ifdef MANUALTIME
  while (state.KeepRunning()) {
    auto start = 1508009805;
	reallocarray_perf();
    auto end   = 1508009846;
    auto elapsed_seconds = end - start;
    state.SetIterationTime(elapsed_seconds);
  }
#else
  while (state.KeepRunning()) reallocarray_perf();
#endif
}

#ifdef MANUALTIME
BENCHMARK(BM_reallocarray)->UseManualTime();
#else
BENCHMARK(BM_reallocarray);
#endif
#endif /* __OpenBSD__ */

void malloc_perf() {

	void *p = NULL;
	int size = 1 * 1024 * 1024;

	p = malloc(size);
	if (!p)
		perror("malloc error");
	free(p);
}

static void BM_malloc(benchmark::State& state) {
#ifdef MANUALTIME
  while (state.KeepRunning()) {
    auto start = 1508009805;
	malloc_perf();
    auto end   = 1508009846;
    auto elapsed_seconds = end - start;
    state.SetIterationTime(elapsed_seconds);
  }
#else
  while (state.KeepRunning()) malloc_perf();
#endif
}

#ifdef MANUALTIME
BENCHMARK(BM_malloc)->UseManualTime();
#else
BENCHMARK(BM_malloc);
#endif

BENCHMARK_MAIN()
