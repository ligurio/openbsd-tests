FILES = at_hello at_hello1
CFLAGS = -static -O3

build:
	g++ at_hello.c -o at_hello -lbenchmark -lpthread
	g++ at_fork.c -o at_fork -lbenchmark -lpthread
	g++ at_cpuid.c -o at_cpuid -lbenchmark -lpthread
	g++ at_syscall_overhead.c -o at_syscall_overhead -lbenchmark -lpthread
	g++ at_sigbench.c -o at_sigbench -lbenchmark -lpthread
	g++ at_fsync.c -o at_fsync -lbenchmark -lpthread
	#g++ at_sha256.c -o at_sha256 -lbenchmark -lpthread -lssl

clean:
	rm bench-[cs]
