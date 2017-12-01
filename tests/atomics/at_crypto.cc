#include "benchmark/benchmark.h"
#include <stdlib.h>

void BM_arc4random(benchmark::State& state) {
#ifdef __OpenBSD__
    while (state.KeepRunning()) arc4random();
#else
    state.SkipWithError("Unsupported");
#endif /* __OpenBSD__ */
}

BENCHMARK(BM_arc4random);

#ifdef __OpenBSD__
void bcrypt_perf() {
	char buffer[50];
	const char *pref;
	const char string = "OpenBSD is freely available from our download sites."

	if (crypt_newhash(string, pref, buffer, sizeof(buffer)) != 0)
		perror("bcrypt error");
}
#endif

void BM_bcrypt(benchmark::State& state) {
#ifdef __OpenBSD__
    while (state.KeepRunning()) bcrypt_perf();
#else
    state.SkipWithError("Unsupported");
#endif
}

BENCHMARK(BM_bcrypt);

BENCHMARK_MAIN()
