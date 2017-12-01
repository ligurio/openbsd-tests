#include "benchmark/benchmark.h"

static void cpuid(void)
{
    asm volatile ("push %%" "bx; cpuid; pop %%" "bx"
                  : : : "eax", "ecx", "edx");
}

static void BM_cpuid(benchmark::State& state) {
    while (state.KeepRunning()) {
		benchmark::DoNotOptimize(cpuid);
    }
}

BENCHMARK(BM_cpuid);

BENCHMARK_MAIN()
