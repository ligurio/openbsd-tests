#include "benchmark/benchmark.h"
#include <openssl/sha.h>

#define BASIC_BENCHMARK_TEST(x) BENCHMARK(x)->Arg(8)->Arg(512)->Arg(8192)

void BM_hello(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(state.iterations());
	  }
}
BENCHMARK(BM_hello);
BENCHMARK(BM_hello)->ThreadPerCpu();

bool BM_SHA256(void* input, unsigned long length, unsigned char* md)
{
    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, (unsigned char*)input, length))
        return false;

    if(!SHA256_Final(md, &context))
        return false;

    return true;
}

BENCHMARK(BM_SHA256);
BENCHMARK(BM_SHA256("sergeyb", 8))->ThreadPerCpu();

BENCHMARK_MAIN()
