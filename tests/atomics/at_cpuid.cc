#include "benchmark/benchmark.h"

/*
 * http://wiki.osdev.org/CPUID
 * https://en.wikipedia.org/wiki/CPUID
 */

void cpuid() {

    unsigned int index = 0;
    unsigned int regs[4];

    __asm__ __volatile__(
#if defined(__x86_64__) || defined(_M_AMD64) || defined (_M_X64)
        "pushq %%rbx     \n\t" /* save %rbx */
#else
        "pushl %%ebx     \n\t" /* save %ebx */
#endif
        "cpuid            \n\t"
        "movl %%ebx ,%[ebx]  \n\t" /* write the result into output var */
#if defined(__x86_64__) || defined(_M_AMD64) || defined (_M_X64)
        "popq %%rbx \n\t"
#else
        "popl %%ebx \n\t"
#endif
        : "=a"(regs[0]), [ebx] "=r"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
        : "a"(index));
}

static void BM_cpuid(benchmark::State& state) {
    while (state.KeepRunning()) {
		benchmark::DoNotOptimize(cpuid);
    }
}

BENCHMARK(BM_cpuid)->Threads(8);
BENCHMARK(BM_cpuid)->ThreadRange(1, 32);
BENCHMARK(BM_cpuid)->ThreadPerCpu();

BENCHMARK_MAIN()
