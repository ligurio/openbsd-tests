#include "benchmark/benchmark.h"

/*
 * http://wiki.osdev.org/CPUID
 * https://en.wikipedia.org/wiki/CPUID
 */

void cpuid() {

    int a, b;

    for (a = 0; a < 5; a++)
    {
      __asm__("cpuid"
              :"=a"(b)                 // EAX into b (output)
              :"0"(a)                  // a into EAX (input)
              :"%ebx","%ecx","%edx");  // clobbered registers

      printf("The code %i gives %i\n", a, b);
    }
}

static void BM_cpuid(benchmark::State& state) {
    while (state.KeepRunning()) {
		benchmark::DoNotOptimize(cpuid);
    }
}

BENCHMARK(BM_cpuid);

BENCHMARK_MAIN()
