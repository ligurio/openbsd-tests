#include "benchmark/benchmark.h"

/*
 * syscall. This test measures round-trip transitions from userlevel to
 * supervisor-level via the syscall and sysret instructions.  The software VMM
 * introduces a layer of code and an extra privilege transition, requiring
 * approximately 2000 more cycles than a native system call. In the hardware
 * VMM, system calls execute without VMM intervention, so as we expect, the
 * hardware VMM executes system calls at native speed.
 */

void syscall_perf() {

}

void BM_syscall(benchmark::State& state) {
	  while (state.KeepRunning()) syscall_perf();
}

BENCHMARK(BM_syscall);

/* in. We execute an in instruction from port 0x80, the BIOS POST port. Native
 * execution accesses an off-CPU register in the chipset, requiring 3209
 * cycles. The software VMM, on the other hand, translates the in into a short
 * sequence of instructions that interacts with the virtual chipset model.
 * Thus, the software VMM executes this instruction fifteen times faster than
 * native. The hardware VMM must perform a vmm/guest round trip to complete the
 * I/O operation. This transition causes in to consume 15826 cycles in the
 * tested system.
 */

void in() {

}

void BM_in(benchmark::State& state) {
	  while (state.KeepRunning()) in();
}

BENCHMARK(BM_in);

/* cr8wr. %cr8 is a privileged register that determines which pending
 * interrupts can be delivered. Only %cr8 writes that reduce %cr8 below the
 * priority of the highest pending virtual interrupt cause an exit. Our FrobOS
 * test never takes interrupts so no %cr8 write in the test ever causes an
 * exit. As with syscall, the hardware VMM’s performance is similar to native.
 * The software VMM translates %cr8 writes into a short sequence of simple
 * instructions, completing the %cr8 write in 35 cycles, about four times
 * faster than native.
 */

void cr8wr() {

}

void BM_cr8wr(benchmark::State& state) {
	  while (state.KeepRunning()) cr8wr();
}

BENCHMARK(BM_cr8wr);

/* call/ret. BT slows down indirect control flow. We target this overhead by
 * repeatedly calling a subroutine. Since the hardware VMM executes calls and
 * returns without modification, the hardware VMM and native both execute the
 * call/return pair in 11 cycles. The software VMM introduces an average
 * penalty of 40 cycles, requiring 51 cycles.
 */

void callret() {

}

void BM_callret(benchmark::State& state) {
	  while (state.KeepRunning()) callret();
}

BENCHMARK(BM_callret);

/* pgfault. In both VMMs, the software MMU interposes on both true and hidden
 * page faults. This test targets the overheads for true page faults. While
 * both VMM paths are logically similar, the software VMM (3927 cycles)
 * performs much better than the hardware VMM (11242 cycles). This is due
 * mostly to the shorter path whereby the software VMM receives control; page
 * faults, while by no means cheap natively (1093 cycles on this hardware), are
 * faster than a vmrun/exit round-trip.
 */

void pgfault() {

}

void BM_pgfault(benchmark::State& state) {
	  while (state.KeepRunning()) pgfault();
}

BENCHMARK(BM_pgfault);

/* divzero. Division by zero has fault semantics similar to those of page
 * faults, but does not invoke the software MMU. While division by zero is
 * uncommon in guest workloads, we include this nanobenchmark to clarify the
 * pgfault results. It allows us to separate out the virtualization overheads
 * caused by faults from the overheads introduced by the virtual MMU. As
 * expected, the hardware VMM (1014 cycles) delivers near native performance
 * (889 cycles), decisively beating the software VMM (3223 cycles).
 */

void divzero() {

}

void BM_divzero(benchmark::State& state) {
	  while (state.KeepRunning()) divzero();
}

BENCHMARK(BM_divzero);

/* ptemod. Both VMMs use the shadowing technique described in Section 2.4 to
 * implement guest paging with trace-based coherency.  The traces induce
 * significant overheads for PTE writes, causing very high penalties relative
 * to the native single cycle store. The software VMM adaptively discovers the
 * PTE write and translates it into a small program that is cheaper than a trap
 * but still quite costly.  This small program consumes 391 cycles on each
 * iteration. The hardware VMM enters and exits guest mode repeatedly, causing
 * it to perform approximately thirty times worse than the software VMM,
 * requiring 12733 cycles.
 */

void ptemod() {

}

void BM_ptemod(benchmark::State& state) {
	  while (state.KeepRunning()) ptemod();
}

BENCHMARK(BM_ptemod);

BENCHMARK_MAIN()
