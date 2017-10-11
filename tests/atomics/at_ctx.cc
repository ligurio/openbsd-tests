#include "benchmark/benchmark.h"
#include <unistd.h>
#include <sys/syscall.h>

/*
 * The system-call overhead benchmark measures the time for the operating
 * system to switch from unprivileged user mode to privileged kernel mode and
 * returning to unprivileged user mode. This metric is performed by measuring
 * the time to execute the getpid system call, which is the simplest system
 * call available in the operating system.
 *
 * The context-switch time benchmark measures the time for the operating system
 * to switch from the execution context of a process to another. A context
 * switch requires the kernel to save the address space and CPU registers of
 * the current process and load them for the next process. The kernel must also
 * manage coherency of the CPU caches. This metric is performed by creating two
 * processes connected with a bi-directional pipe. A one-byte token is passed
 * alternately between the two processes, causing a context switch to each
 * process to services access to the pipe. 
 *
 */

#define ITERATIONS	1000000

/*
 * The idea is to make a cheap system call many times in a row, time how long
 * it took, and compute the average time spent per syscall. The cheapest system
 * call on Linux these days seems to be gettid. Turns out, this was a naive
 * approach since system calls don't actually cause a full context switch anymore
 * nowadays, the kernel can get away with a "mode switch" (go from user mode to
 * kernel mode, then back to user mode). vmstat wouldn't show a noticeable
 * increase in number of context switches.
 */

// https://github.com/tsuna/contextswitch
// http://blog.tsunanet.net/2010/11/how-long-does-it-take-to-make-context.html
// ftp://ftp.netbsd.org/pub/NetBSD/misc/gmcgarry/bench/ctxbench.tar.gz

void time_syscall(void) {

	int i;
	pid_t pid;

	if (syscall(SYS_gettid) <= 1) {
	exit(2);
	}

/*
	for (i=0; i<ITERATIONS; i++) {
		pid = getpid();
	}
*/
}

void BM_time_syscall(benchmark::State& state) {
	  while (state.KeepRunning()) {
		      benchmark::DoNotOptimize(time_syscall);
	  }
}

/* 
 * futex is the low level Linux-specific primitive used by most threading
 * libraries to implement blocking operations such as waiting on a contended
 * mutexes, semaphores that run out of permits, condition variables and friends.
 * If you would like to know more, go read Futexes Are Tricky by Ulrich Drepper.
 * Anyways, with a futex, it's easy to suspend and resume processes. Test forks
 * off a child process, and the parent and the child take turn waiting on the
 * futex. When the parent waits, the child wakes it up and goes on to wait on the
 * futex, until the parent wakes it and goes on to wait again. Some kind of a
 * ping-pong "I wake you up, you wake me up...".
 *
 * http://people.redhat.com/drepper/futex.pdf
 */

void time_ctxswws(void) {

	// https://github.com/tsuna/contextswitch/blob/master/timectxswws.c
}


void timet_ctxsw(void) {

	// https://github.com/tsuna/contextswitch/blob/master/timetctxsw.c
}

void time_ctxsw(void) {

	// https://github.com/tsuna/contextswitch/blob/master/timectxsw.c
}

void timet_ctxsw2(void) {

	// https://github.com/tsuna/contextswitch/blob/master/timetctxsw2.c
}

BENCHMARK(BM_time_syscall);
BENCHMARK(BM_time_syscall)->Threads(8);
BENCHMARK(BM_time_syscall)->ThreadRange(1, 32);
BENCHMARK(BM_time_syscall)->ThreadPerCpu();

BENCHMARK_MAIN()
