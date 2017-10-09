#include "benchmark/benchmark.h"
#include <openssl/sha.h>

bool sha256(void* input, unsigned long length)
{
    unsigned char *md;

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, (unsigned char*)input, length))
        return false;

    if(!SHA256_Final(md, &context))
        return false;

    return true;
}

void BM_SHA256(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(sha256);
	  }
}

BENCHMARK(BM_SHA256);
//BENCHMARK(BM_SHA256("sergeyb", 8))->ThreadPerCpu();

BENCHMARK_MAIN()
