#include "benchmark/benchmark.h"
#include <stdlib.h>
#include <string.h>

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

size_t b_malloc_sparse()
{
	void *p[10000];
	size_t i;
	for (i=0; i<sizeof p/sizeof *p; i++) {
		p[i] = malloc(4000);
		memset(p[i], 0, 4000);
	}
	for (i=0; i<sizeof p/sizeof *p; i++)
		if (i%150) free(p[i]);
	return 0;
}

static void BM_malloc_sparse(benchmark::State& state) {
  while (state.KeepRunning()) b_malloc_sparse();
}

BENCHMARK(BM_malloc_sparse);

size_t b_malloc_bubble()
{
	void *p[10000];
	size_t i;
	for (i=0; i<sizeof p/sizeof *p; i++) {
		p[i] = malloc(4000);
		memset(p[i], 0, 4000);
	}
	for (i=0; i<sizeof p/sizeof *p-1; i++)
		free(p[i]);
	return 0;
}

static void BM_malloc_bubble(benchmark::State& state) {
  while (state.KeepRunning()) b_malloc_bubble();
}

BENCHMARK(BM_malloc_bubble);

size_t b_malloc_tiny1()
{
	void *p[10000];
	size_t i;
	for (i=0; i<sizeof p/sizeof *p; i++) {
		p[i] = malloc((i%4+1)*16);
	}
	for (i=0; i<sizeof p/sizeof *p; i++) {
		free(p[i]);
	}
	return 0;
}

static void BM_malloc_tiny1(benchmark::State& state) {
  while (state.KeepRunning()) b_malloc_tiny1();
}

BENCHMARK(BM_malloc_tiny1);

size_t b_malloc_tiny2()
{
	void *p[10000];
	size_t i;
	for (i=0; i<sizeof p/sizeof *p; i++) {
		p[i] = malloc((i%4+1)*16);
	}
	for (i=1; i; i = (i+57)%(sizeof p/sizeof *p))
		free(p[i]);
	return 0;
}

static void BM_malloc_tiny2(benchmark::State& state) {
  while (state.KeepRunning()) b_malloc_tiny2();
}

BENCHMARK(BM_malloc_tiny2);

size_t b_malloc_big1()
{
	void *p[2000];
	size_t i;
	for (i=0; i<sizeof p/sizeof *p; i++) {
		p[i] = malloc((i%4+1)*16384);
	}
	for (i=0; i<sizeof p/sizeof *p; i++) {
		free(p[i]);
	}
	return 0;
}

static void BM_malloc_big1(benchmark::State& state) {
  while (state.KeepRunning()) b_malloc_big1();
}

BENCHMARK(BM_malloc_big1);

size_t b_malloc_big2()
{
	void *p[2000];
	size_t i;
	for (i=0; i<sizeof p/sizeof *p; i++) {
		p[i] = malloc((i%4+1)*16384);
	}
	for (i=1; i; i = (i+57)%(sizeof p/sizeof *p))
		free(p[i]);
	return 0;
}

static void BM_malloc_big2(benchmark::State& state) {
  while (state.KeepRunning()) b_malloc_tiny2();
}

BENCHMARK(BM_malloc_big2);

BENCHMARK_MAIN()
